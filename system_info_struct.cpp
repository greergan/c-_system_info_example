#include <string>
#include <system_error>

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

struct system_info {
	struct _host{
		char _tmp[255];
		int error = 0;
		std::string name;
		_host() {
			if(gethostname(_tmp, 255) == 0)
				name = _tmp;
			if(getdomainname(_tmp, 255) == 0 && _tmp[0] != '\0') {
				name += '.';
				name += _tmp;
			}
			if(errno)
				throw std::system_error(errno, std::system_category());
		}
	}host;
	struct {
		pid_t id = syscall(SYS_gettid);
		pid_t parent_id = syscall(SYS_getpid);
	}process;
	struct {
		const int count = sysconf(_SC_NPROCESSORS_ONLN);
		const int online = sysconf(_SC_NPROCESSORS_ONLN);
	}cpu;
	struct {
		struct {
			const unsigned long size = sysconf(_SC_PAGE_SIZE);
		}page;
		struct {
			const unsigned long count = sysconf(_SC_PHYS_PAGES);
			const unsigned long available = sysconf(_SC_AVPHYS_PAGES);
		}pages;
		const unsigned long total = page.size * pages.count;
	}memory;
};

 
int main() {
	system_info system;
    printf("pid %u of parent_id %u: %u cores: %ld total memory: host %s\n\n", system.process.id, system.process.parent_id,
							system.cpu.count, system.memory.total,
							system.host.name.c_str());
	return 0;
}
