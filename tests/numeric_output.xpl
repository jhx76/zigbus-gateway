# xpl-cmnd
# {
# hop=1
# source=<some controller>
# target=zigbus-module.<moduleId>:<networkId>
# }
# control.basic
# {
# device=numeric_output(<io>)
# type=output
# current=enable|disable|on|off
# }

xpl-cmnd
{
hop=1
source=zigbus-controller.01
target=zigbus-module.1:1
}
control.basic
{
device=numeric_output(S1)
type=output
current=enable
time=30
unit=sec|dsec
}

