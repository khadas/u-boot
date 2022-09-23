 #!/bin/bash

EXEC_BASEDIR=$(dirname $(readlink -f $0))

#
# Settings
#
VERSION=0.1

usage() {
    cat << EOF
Usage: $(basename $0) --help
       $(basename $0) --version
       $(basename $0) --audio-id audio_id_value \\
                       --soc [axg | txhd | g12a | g12b | sm1 | tl1 | tm2 | a1 | c1 |c2 | t5 | t5d | t5w | sc2 | t7 | s4 | t3 | s4d | p1 | a5 | c3] \\
                       -o audio_id.efuse
EOF
    exit 1
}

function generate_audio_id_pattern() {
    local argv=("$@")
    local i=0
     # Parse args
    i=0
    while [ $i -lt $# ]; do
        arg="${argv[$i]}"
        #echo "i=$i argv[$i]=${argv[$i]}"
        i=$((i + 1))
        case "$arg" in
            --soc)
                soc="${argv[$i]}" ;;
            --audio-id)
                audio_id_value="${argv[$i]}" ;;
           -o)
                output="${argv[$i]}" ;;
            *)
                echo "Unknown option $arg"; exit 1
                ;;
        esac
        i=$((i + 1))
    done

    if [ -z $audio_id_value ]; then
        echo Error: invalid audio_id_value
        exit 1
    fi

    if [ -z $output ]; then
        echo Error: invalid output
        exit 1
    fi

    if [ ${soc} == "s4d" ]; then
        soc=s4
    fi

    if [ ${soc} == "sc2" ] || [ ${soc} == "t7" ] || [ ${soc} == "s4" ] || [ ${soc} == "t3" ] \
       || [ ${soc} == "p1" ] || [ ${soc} == "a5" ] || [ ${soc} == "c3" ]; then
        ${EXEC_BASEDIR}/${soc}/bin/efuse-gen.sh --audio-id ${audio_id_value} -o ${output}
    elif [ "$soc" == "axg" ] || [ "$soc" == "txhd" ] || [ "$soc" == "g12a" ] \
       || [ "$soc" == "sm1" ] || [ "$soc" == "g12b" ] || [ "$soc" == "tl1" ] || [ "$soc" == "tm2" ] \
       || [ "$soc" == "a1" ] || [ "$soc" == "c1" ] || [ "$soc" == "c2" ] \
       || [ "$soc" == "t5" ] || [ "$soc" == "t5d" ] || [ "$soc" == "t5w" ]; then
        ${EXEC_BASEDIR}/stool/efuse.sh --audio-id ${audio_id_value} --soc ${soc} -o ${output}
    else
        echo Error: invalid soc [$soc]
        exit 1
    fi
}

parse_main() {
    case "$@" in
        --help)
            usage
            ;;
        --version)
            echo "$(basename $0) version $VERSION"
            ;;
        *--audio-id*)
            generate_audio_id_pattern "$@"
            ;;
        *)
            usage "$@"
            ;;
    esac
}

parse_main "$@"
