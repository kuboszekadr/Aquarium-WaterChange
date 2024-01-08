# Logger

The logger class is designed to be craeted separatelly inside each module separately, thanks to that, module name can be sent do output stream. 

Please note, that at the moment `_log_level` is only placeholder value for further development in the future.

Please also note, that there is not default stream set, so user must define where to send data on the top level. All instanes of `Logger` class will send its' output there.