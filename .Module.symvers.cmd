cmd_/home/sunwei/cdev/Module.symvers := sed 's/\.ko$$/\.o/' /home/sunwei/cdev/modules.order | scripts/mod/modpost -m -a  -o /home/sunwei/cdev/Module.symvers -e -i Module.symvers   -T -
