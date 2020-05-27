make compiler
make release

files=""
for entry in "./OS"/*
do
    a="${entry##*/}"
    #echo ${a%.*}
    ./compiler "$entry" ${a%.*}
    files="${files} ${a%.*}"
done

python3 OSInstaller.py ${files}
