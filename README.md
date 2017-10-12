A better readme to come later. In the meantime, I just want to document this helpful bash code so I don't lose it.

```
#This generates multiples of the comand below to address multiple IP addresses
i=1
erSpiCmd=""
while [ $i -le 6 ]; do
  erSpiCmd=$erSpiCmd"echo 'Syncing Emergency Room to 192.168.1.20"$i"...' && sshpass -p 'password' rsync -avzh --delete --exclude '.git' --exclude '.gitignore' --exclude '*.xcodeproj' --exclude 'openFrameworks-Info.plist' --exclude 'Project.xcconfig' --exclude '*.app' --exclude 'bin/data/AudioUnitPresets' --exclude 'bin/data/log' --exclude 'bin/data/test' --exclude '.DS_Store' --exclude 'obj' --exclude 'bin/emergencyroom' --exclude 'bin/readMe.txt' /Users/username/projects/emergency-room/of098/apps/myApps/emergencyroom/ pi@192.168.1.20"$i":/home/pi/emergency-room/of098/apps/myApps/emergencyroom/ && sshpass -p 'password' rsync -avzh --delete --exclude '.DS_Store' /Users/username/projects/emergency-room/er-media/ pi@192.168.1.20"$i":/home/pi/emergency-room/er-media/ && echo '' && "
  i=`expr $i + 1`
done

erSpiCmd=$erSpiCmd"echo 'Done.'"

eval "alias spi=\""$erSpiCmd"\""
#echo "alias spi=\""$erSpiCmd"\""

#That's it. This is the source command addressing 192.168.1.202:
#alias spi="sshpass -p 'password' rsync -avzh --delete --exclude '.git' --exclude '.gitignore' --exclude '*.xcodeproj' --exclude 'openFrameworks-Info.plist' --exclude 'Project.xcconfig' --exclude '*.app' --exclude 'bin/data/AudioUnitPresets' --exclude 'bin/data/log' --exclude 'bin/data/test' --exclude '.DS_Store' --exclude 'obj' --exclude 'bin/emergencyroom' --exclude 'bin/readMe.txt' /Users/username/projects/emergency-room/of098/apps/myApps/emergencyroom/ pi@192.168.1.202:/home/pi/emergency-room/of098/apps/myApps/emergencyroom/ && sshpass -p 'password' rsync -avzh --delete --exclude '.DS_Store' /Users/username/projects/emergency-room/er-media/ pi@192.168.1.202:/home/pi/emergency-room/er-media/"
```