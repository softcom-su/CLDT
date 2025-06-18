make clean
if make ${TARGET}
    then
	mv ${IMAGE} ${RESULT};
    else
	echo -e '\a';
fi
echo ${RESULT} done
