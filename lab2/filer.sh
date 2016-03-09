while [[ : ]]; do
  echo ' 1 - filelist'; echo ' 2 - delete'
  echo ' 3 - rename'; echo ' 4 - exit'
  read -p "enter command... " cmd
  case $cmd in
    1)  echo $(ls);;
    2)
      while [[ : ]]; do
        read -p 'enter file name...'  fileName
        if [[ -z $fileName ]]; then
          break
        fi
        if [[ -e $fileName && -f $fileName ]]; then
          rm $fileName
          echo 'file'$fileName' deleted'
          break
        else
          echo 'no such file, try again...'
        fi
      done;;
    3)
      while [[ : ]]; do
        read -p 'enter file name...'  fileName
        if [[ -z $fileName ]]; then
          break
        fi
        # check if file exist
        if [[ -e $fileName && -f $fileName ]]; then
          while [[ : ]]; do
            read -p 'enter new file name...'  newFileName
            if [[ -z $newFileName ]]; then
              break
            fi
            if [[ -e $newFileName ]]; then
              echo 'file with this name already exists, try again...'
            else
              mv $fileName $newFileName
              echo $fileName' renamed to ' $newFileName
              break
            fi
          done
          break
        else
          echo 'no such file, try again...'
        fi
      done;;
    4)
      echo 'gl hf'
      exit;;
  esac
done
