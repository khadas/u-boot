#!/usr/bin/env python3

import sys
import os
import re
import requests
from requests.auth import HTTPBasicAuth
from urllib.parse import urljoin
import urllib.request
import json
import time
from os.path import expanduser
import jenkins


serverRootUrl = "https://jenkins-sh.amlogic.com/job/Security/job/"
homeConfigFilePath = "~/.sign.cfg"
types = ["ta", "vmxta", "bl32", "bl31", "bl2", "bl2e", "bl2x", "bl40", "aucpufw", "vdecfw"]
casProviders = ["", "VMX", "nagra", "nagra-dev", "vo", "gs-dev", "gs", "irdeto"]
ddrTypes = ["ddr4", "lpddr4", "ddr3", "lpddr3", "lpddr4_lpddr5"]
chipVariants = ["general", "nocs-jts-ap", "nocs-prod"]

user = ""
password = ""
auth = None
server = None


def init():
    global user
    global password
    global auth
    global server
    expandedHomeConfigFilePath = expanduser(homeConfigFilePath)
    configPyPath = os.path.join(sys.path[0], "config.py")
    if os.path.exists(expandedHomeConfigFilePath):
        configFilePath = expandedHomeConfigFilePath
    elif os.path.exists(configPyPath):
        configFilePath = configPyPath
    else:
        print(
            "Can't find configuration file. Please create configuration file .sign.cfg at your home directory."
        )
        exit(1)

    with open(configFilePath, "r") as configFile:
        while True:
            line = configFile.readline()
            if not line:
                break
            words = line.split("=")
            if words[0].strip() == "user":
                user = words[1].strip().replace("'", "")
            elif words[0].strip() == "password":
                password = words[1].strip().replace("'", "")
    auth = HTTPBasicAuth(user, password)
    server = jenkins.Jenkins(
        "https://jenkins-sh.amlogic.com", username=user, password=password
    )


def get_args():
    from argparse import ArgumentParser

    parser = ArgumentParser()

    parser.add_argument("--in", dest="inputFilePath", required=True, help="input file")
    parser.add_argument(
        "--chipAcsFile", dest="chipAcsFilePath", default="null", help="chip acs file"
    )
    parser.add_argument(
        "--out", dest="outFilePath", type=str, default="", help="output signed file"
    )
    parser.add_argument(
        "-v", "--version", action="version", version="%(prog)s 1.0", help="version"
    )
    parser.add_argument("--type", choices=types, default=types[0], required=True)
    parser.add_argument("--chip", type=str)
    parser.add_argument("--taVersion", type=int, default=0)
    parser.add_argument("--marketId", type=str, default="null")
    parser.add_argument("--casProvider", choices=casProviders, default=casProviders[0])
    parser.add_argument("--ddrType", type=str, default=ddrTypes[0])
    parser.add_argument("--chipVariant", choices=chipVariants, default=chipVariants[0])
    parser.add_argument("--keyType", type=str, dest="keyType", default="dev-keys")
    parser.add_argument("--extraArgs", type=str, default="")
    parser.add_argument("--testService", type=int, default=0)

    return parser.parse_args()


def getLastBuildNumber(rootJobUrl):
    url = urljoin(rootJobUrl, "lastBuild/buildNumber")

    response = requests.get(url, auth=auth)

    if response.status_code == 200:
        return response.text
    else:
        print(
            "Fail to get last build number due to the error "
            + str(response.status_code)
        )
        return 0


def getJobRootUrl(type):
    if type == "ta":
        return urljoin(serverRootUrl, "Signing/job/Sign_TA/")
    elif type == "vmxta":
        return urljoin(serverRootUrl, "CAS/job/VMX/job/VMX_TA_Sign/")
    elif type == "bl31":
        return urljoin(serverRootUrl, "Signing/job/Sign_Bl31/")
    elif type == "bl2":
        return urljoin(serverRootUrl, "Signing/job/Sign_Bl2/")
    elif type == "bl32":
        return urljoin(serverRootUrl, "Signing/job/Sign_Bl32/")
    elif type == "aucpufw":
        return urljoin(serverRootUrl, "Signing/job/Sign_AUCPU_FW/")
    elif type == "vdecfw":
        return urljoin(serverRootUrl, "Signing/job/Sign_VDEC_FW/")
    else:  # bl2e, bl2x, bl40
        return urljoin(serverRootUrl, "Signing/job/Sign_Bl2e_Bl2x_Bl40/")


def getJobName(type):
    if type == "ta":
        return "Sign_TA"
    elif type == "bl31":
        return "Sign_Bl31"
    elif type == "bl2":
        return "Sign_Bl2"
    elif type == "bl32":
        return "Sign_Bl32"
    elif type == "aucpufw":
        return "Sign_AUCPU_FW"
    elif type == "vdecfw":
        return "Sign_VDEC_FW"
    else:  # bl2e, bl2x, bl40
        return "Sign_Bl2e_Bl2x_Bl40"


def submitSignJob(
    type,
    chipType,
    inputFilePath,
    chipAcsFilePath,
    taVersion="0",
    marketId="",
    casProvider="",
    chipVariant="",
    ddrType="",
    keyType="dev-keys",
    extraArgs="",
    testService=0,
):

    fileName = os.path.basename(inputFilePath)
    fileParameter = "file"
    uploadFile = {
        fileParameter: (fileName, open(inputFilePath, "rb")),
    }
    url = getJobRootUrl(type) + "buildWithParameters"
    if type == "ta":
        data = {
            "chip_part_number": chipType,
            "ta_version": taVersion,
            "market_id": marketId,
            "testService": testService,
        }
    elif type == "bl32":

        data = {
            "chipPartNumber": chipType,
            "casProvider": casProvider,
            "keyType": keyType,
            "testService": testService,
        }

    elif type == "bl2":
        chipAcsfileName = os.path.basename(chipAcsFilePath)
        uploadFile = {
            fileParameter: (fileName, open(inputFilePath, "rb")),
            "chipAcsFile": (chipAcsfileName, open(chipAcsFilePath, "rb")),
        }
        data = {
            "chipPartNumber": chipType,
            "chipVariant": chipVariant,
            "ddrType": ddrType,
            "keyType": keyType,
            "testService": testService,
        }
    else:  # bl2e, bl2x, bl31, bl40, aucpufw, vdecfw
        data = {
            "chipPartNumber": chipType,
            "keyType": keyType,
            "extraArgs": extraArgs,
            "testService": testService,
        }

    response = requests.post(url, auth=auth, data=data, files=uploadFile)

    if response.status_code == 201:
        print("Sumbit signing job successfully, please wait...")

    else:
        print(
            "Fail to start signing job due to the error: " + str(response.status_code)
        )
        exit(1)


def queryBuildStatus(rootJobUrl, buildNumber):
    url = rootJobUrl + str(buildNumber) + "/api/json?tree=building"

    response = requests.get(url, auth=auth)

    if response.status_code == 200:
        result = json.loads(response.text)
        return str(result["building"])
    else:
        return "NotStart"


def downloadSignedFile(rootJobUrl, buildNumber, inFileDir="", specifiedOutFilePath=""):

    url = rootJobUrl + str(buildNumber) + "/api/json?tree=artifacts[relativePath]"

    response = requests.get(url, auth=auth)

    if response.status_code == 200:
        result = json.loads(response.text)
        if len(result["artifacts"]) == 0:
            print("Fail to build, please check jenkins log for detailed error")
            exit(1)
        relativePath = result["artifacts"][0]["relativePath"]
        # http://127.0.0.1:8080/job/Sign_Bl31/46/artifact/46/output/bl31-payload.bin.signed
        downloadUrl = rootJobUrl + str(buildNumber) + "/artifact/" + "/" + relativePath
        if specifiedOutFilePath == "":
            outFilePath = os.path.join(inFileDir, os.path.basename(relativePath))
        else:
            outFilePath = specifiedOutFilePath
        r = requests.get(downloadUrl, auth=auth)
        with open(outFilePath, "wb") as f:
            f.write(r.content)
        print("Download the signed file at " + outFilePath)
        return 0
    else:
        print("Fail to download the signed file")
        exit(1)
        return 1


def waitForSubmit(type):
    jobName = getJobName(type)

    while True:
        queues = server.get_queue_info()
        inQueue = False
        if queues:
            for queue_job_info in queues:
                if queue_job_info["task"].get("name", "") == jobName:
                    inQueue = True
                    break
        if inQueue:
            time.sleep(1)
            print(
                "Otherone is signing same firmware as you request. Please wait them to complete."
            )
        else:
            print("It is your turn to submit your signing job now.")
            break


def main():
    print(sys.argv)
    init()
    args = get_args()

    rootJobUrl = getJobRootUrl(args.type)

    waitForSubmit(args.type)
    lastBuildNumber = getLastBuildNumber(rootJobUrl)
    submitSignJob(
        type=args.type,
        chipType=args.chip,
        inputFilePath=args.inputFilePath,
        chipAcsFilePath=args.chipAcsFilePath,
        taVersion=args.taVersion,
        marketId=args.marketId,
        casProvider=args.casProvider,
        chipVariant=args.chipVariant,
        ddrType=args.ddrType,
        keyType=args.keyType,
        extraArgs=args.extraArgs,
        testService=args.testService,
    )

    buildNumber = int(lastBuildNumber) + 1
    print("The jenkins build number: " + str(buildNumber))
    while True:
        time.sleep(1)
        building = queryBuildStatus(rootJobUrl, buildNumber)
        print("Building Status= " + str(building))
        if building == "False":
            print("Build is done. Will start to download the signed file")
            break
    inputFileDir = os.path.dirname(args.inputFilePath)
    downloadSignedFile(rootJobUrl, buildNumber, inputFileDir, args.outFilePath)


if __name__ == "__main__":
    main()
