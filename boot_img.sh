#!/bin/bash

[ -e hd.raw ] && rm hd.raw
[ -e hd.vdi ] && rm hd.vdi
echo 'turn img to vbox vdi'
#qemu-img convert daishuos.bin -O raw hd.raw
VBoxManage convertdd daishuos.bin hd.vdi
VBoxManage createvm --name 30day_os --register
VBoxManage storagectl 30day_os --add scsi --name SCSI
VBoxManage storageattach 30day_os --storagectl SCSI --medium hd.vdi --port 0 --type hdd
VBoxManage startvm 30day_os
sleep 10
VBoxManage controlvm 30day_os poweroff
sleep 1
VBoxManage unregistervm 30day_os --delete
