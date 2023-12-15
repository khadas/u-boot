#!/usr/bin/env python3
#
#Copyright (c) 2023 Amlogic, Inc. All rights reserved.
#

import argparse
import os
import binascii
import uuid

def get_args():
    parser = argparse.ArgumentParser(
            description='Convert user ta to c file')
    parser.add_argument(
            '--dir',
            required=True,
            help='Path for dest c file')
    return parser.parse_args()

def fmt_uuid(ta_name):
    rst = {}
    t_uuid_clk_seq_nd = ''

    rst['time_low'] = ta_name.split('-')[0]
    rst['time_mid'] = ta_name.split('-')[1]
    rst['time_hi_v'] = ta_name.split('-')[2]
    rst['clk_seq_node'] = ta_name.split('-')[3] + ta_name.split('-')[4]

    t_uuid = '\t{ ' + '0x' + rst['time_low'] + ', 0x' + rst['time_mid'] + \
             ', 0x' + rst['time_hi_v'] + ', \\\n\t\t{'
    for i in range(0, len(rst['clk_seq_node']), 2):
        if i >= (len(rst['clk_seq_node']) - 2):
            t_uuid_clk_seq_nd += ' 0x' + rst['clk_seq_node'][i:i+2] + '} }'
        else:
            t_uuid_clk_seq_nd += ' 0x' + rst['clk_seq_node'][i:i+2] + ','

    t_uuid += t_uuid_clk_seq_nd

    return t_uuid

def parse_uuid(ta_name):
    t_uuid = uuid.UUID(ta_name)
    print(t_uuid)
    t_tm_low = '0x{:08x}, '.format(t_uuid.time_low)
    t_tm_mid = '0x{:04x}, '.format(t_uuid.time_mid)
    t_tm_hi_v = '0x{:04x}, '.format(t_uuid.time_hi_version)
    t_clk_seq = '0x{0:02x}, 0x{1:02x}, '.format(t_uuid.clock_seq_hi_variant,
                    t_uuid.clock_seq_low)
    t_clk_nd = '0x{:012x}, '.format(t_uuid.node)
    print(t_tm_low, t_tm_mid, t_tm_hi_v, t_clk_seq, t_clk_nd)

def gen_c_file_head(dst_f):
    f_licens = '/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */\n' + \
               '/*\n * Copyright (c) 2023 Amlogic, Inc. All rights reserved.\n */\n\n'
    f_inc = '#include<common.h>\n#include<linux/types.h>\n#include<tee.h>\n\n'
    f = open(dst_f, 'w')
    f.write(f_licens + f_inc)
    f.close()

def gen_c_file_body(f_ta, dst_f, flag):
    rst_dict = {}
    ta_uuid_name = f_ta.split('/')[-1].split('.')[0]
    dst_ta_ctx = 'ta_ctx_' + flag
    dst_ta_name = 'TA_UUID_' + flag
    def_uuid = fmt_uuid(ta_uuid_name)
    #parse_uuid(ta_uuid_name)

    with open(f_ta, 'rb') as indata:
        bytes = indata.read()
        size = len(bytes)

    f = open(dst_f, 'a')
    f.write('#define ' + dst_ta_name + ' \\\n' + def_uuid + '\n\n')
    f.write('const uint8_t ' + dst_ta_ctx + '[] = {\n')

    i = 0
    while i < size:
        if i % 8 == 0:
            f.write('\t\t')
        #for python3
        f.write(hex(bytes[i]) + ',')

        #for python2
        #f.write('0x' + binascii.b2a_hex(bytes[i]) + ',')
        i = i + 1
        if i % 8 == 0 or i == size:
            f.write('\n')
        else:
            f.write(' ')
    f.write('};\n\n')

    f.close()
    indata.close()

    rst_dict['file_name'] = dst_f[8:]
    rst_dict['uuid_name'] = dst_ta_name
    rst_dict['ta_name'] = dst_ta_ctx
    return rst_dict

def gen_c_file_tail(ta_tb, dst_f):
    f = open(dst_f, 'a')

    f_u_tb = 'struct tee_optee_uta optee_uta_tb[] = {\n'
    if len(ta_tb) > 0:
        for ta in ta_tb:
            f_u_tb += '\t{.uuid = ' + ta['uuid_name'] + ', .sz = ARRAY_SIZE(' \
                  + ta['ta_name'] + '), .ta_payload = (void *)' \
                  + ta['ta_name'] + '},\n'
    f_u_tb += '\t{}\n};\n\n'
    f_fun = 'int tee_optee_get_uta(struct tee_optee_uta **uta, size_t *uta_num)\n' \
            + '{\n\t*uta = optee_uta_tb;\n\t*uta_num = ARRAY_SIZE(optee_uta_tb);\n' \
            + '\n\treturn 0;\n}\n'

    f.write(f_u_tb + f_fun)
    f.close()

def main():
    ta_tb = []
    flag = 0
    args = get_args()
    dst_f = args.dir + "/uta.c"

    gen_c_file_head(dst_f)
    for root,dirs,files in os.walk(args.dir):
        for f_ta in files:
            f = os.path.join(root,f_ta)
            if f.endswith(".ta"):
                flag += 1
                rst = gen_c_file_body(f, dst_f, str(flag))
                ta_tb.append(rst)
    gen_c_file_tail(ta_tb, dst_f)

if __name__ == "__main__":
    main()
