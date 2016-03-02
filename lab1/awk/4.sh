ls test | grep 'unix' | awk -F"_" '{print $2}' | sort
