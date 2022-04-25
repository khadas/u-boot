ARCH_DIRS="n200"
SOC_DIRS="sc2 t5 t5d t7 s4 t3 p1 t5w a5"

function list_boards()
{
	for arch in ${ARCH_DIRS}; do
		for soc in ${SOC_DIRS}; do
			for board in demos/amlogic/${arch}/${soc}/*; do
				if [ -d ${board} -a -e ${board}/config.mk ]; then
					echo `basename "${board}"`
				fi
			done
		done
	done

	for board in demos/amlogic/customer/*; do
		if [ -d ${board} -a -e ${board}/config.mk ]; then
			echo `basename "${board}"`
		fi
	done
}

function get_arch()
{
	board="$1"
	flag=0
	for arch in ${ARCH_DIRS}; do
		for soc in ${SOC_DIRS}; do
			if [ -d demos/amlogic/${arch}/${soc}/${board} -a -e demos/amlogic/${arch}/${soc}/${board}/config.mk ]; then
				echo ${arch}
				flag=1
				break
			fi
		done
		if [ $flag == 1 ]; then
			break
		fi
	done

	if [ $flag == 0 -a -d demos/amlogic/customer/${board} -a -e demos/amlogic/customer/${board}/config.mk ]; then
		echo "customer"
	fi
}

function get_soc()
{
	board="$1"
	flag=0
	for arch in ${ARCH_DIRS}; do
		for soc in ${SOC_DIRS}; do
			if [ -d demos/amlogic/${arch}/${soc}/${board} -a -e demos/amlogic/${arch}/${soc}/${board}/config.mk ]; then
				echo ${soc}
				flag=1
				break
			fi
		done
		if [ $flag == 1 ]; then
			break
		fi
	done

	if [ $flag == 0 -a -d demos/amlogic/customer/${board} -a -e demos/amlogic/customer/${board}/config.mk ]; then
		echo ""
	fi
}

