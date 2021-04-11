# README for process_log system call

##### By Marshall Aiken

##### CAP4600, Project1

### Description

Once installed, this module will allow processes to log messages to the kernel log while in user space. These message logs can be viewable using `dmesg`. Messages will be formatted as: `$log_level_name [ $executable , $pid ]: $message` 

The module adds a kernel wide variable `proc_log_level` that is initialized to 0 on startup and will persist until shutdown. The log level can range from 0 to 7, ranging from most to least serious log level respectively. Any process can read the current log level, and only processes running as a superuser may set the log level. Any process may also attempt to log a message, but if the requested log level of the message is greater than the current log level, the message will be ignored.

This module also includes a static library that once included, will allow any process to access the necessary system calls. The functions are:

- `int get_proc_log_level()` 
  - Returns the current log level if successful and -1 if failed.
- `int set_proc_log_level(int new_level)` 
  - Will set the log level to `new_level` if called by a superuser and `new_level` is within 0-7. Returns `new_level` if successful and -1 if failed.
- `int proc_log_message(int level, char *message)` 
  - Will attempt to log a message `message` at log level `level`.  If `level` is greater than the current log level, the message will not be logged. Will return `level` if successful and -1 if failed.

### Design

To accomplish the intended functionality, three system calls were added to the kernel.

- `435 get_process_log`
- `436 set_process_log`
- `437 log_message`

These system calls were listed, prototyped, and defined respectively in the following files:

- arch/x86/entry/syscalls/syscall_64.tbl
- inclued/linux/syscalls.h
- kernel/sys.c

The variable `proc_log_level` was defined and exported in kernel/sys.c. `proc_log_level` is then initialized to 0 in init/main.c at the start of the function `kernel_init`. 

Within the system calls themselves, `get_process_log` simply returns `proc_log_level`. `set_process_log` checks the calling processes' privileges and validates the level range before setting the log level to the given value. `log_message` will check the current log level against the requested log level, if the requested log level is valid it will log a message via the `printk` function at the corresponding kernel level. The Kernel Level names and their corresponding log level and Process Level Name can be seen here:

| Kernel Level Name | Description                                      | #    | Process Level Name |
| ----------------- | ------------------------------------------------ | ---- | ------------------ |
| KERN_EMERG        | Emergency / Crash Imminent (no process logging)  | 0    | PROC_OVERRIDE      |
| KERN_ALERT        | Immediate Action Required                        | 1    | PROC_ALERT         |
| KERN_CRIT         | Critical/Serious Failure Occurred                | 2    | PROC_CRITICAL      |
| KERN_ERR          | Error Condition Occurred                         | 3    | PROC_ERROR         |
| KERN_WARNING      | Warning; recoverable, but may indicate problems  | 4    | PROC_WARNING       |
| KERN_NOTICE       | Notable, but not serious (e.g., security events) | 5    | PROC_NOTICE        |
| KERN_INFOR        | Informational (e.g. initialization / shutdown)   | 6    | PROC_INFO          |
| KERN_DEBUG        | Debug messages                                   | 7    | PROC_DEBUG         |

### Verification

To verify the functionality of the module, first apply the `.diff` file while in the directory of the kernel. This should add the necessary system calls and the `proc_log_level` variable. Make, install, and modules install after applying the changes. Once modules_install has finished, restart the machine. This is necessary so that the changes are properly applied and so `proc_log_level` can be initialized to 0 on startup.

After rebooting, unzip the `process_log.tar.gz` file into a `src` directory. Enter the new `process_log` directory, run `make`, and exit the directory. Have your testing code file in the same directory as the `process_log` directory. Within your code file, `#include 'process_log/process_log.h'` to link to the library. Within your code call any of the functions listed in the description of this README. To compile, link the library by using the following command:

`cc -o program_name sourcefile.c -L ./process_log -lprocess_log`

