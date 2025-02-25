
#include <stdint.h>
#include <stdlib.h>
#include <bits/ensure.h>
#include <mlibc/elf/startup.h>

// defined by the POSIX library
void __mlibc_initLocale();

extern "C" int main(int argc, char *argv[], char *env[]);

extern "C" uintptr_t *__dlapi_entrystack();

extern char **environ;
static mlibc::exec_stack_data __mlibc_stack_data;

struct LibraryGuard {
	LibraryGuard();
};

static LibraryGuard guard;

LibraryGuard::LibraryGuard() {
	__mlibc_initLocale();

	// Parse the exec() stack.
	mlibc::parse_exec_stack(__dlapi_entrystack(), &__mlibc_stack_data);
	mlibc::set_startup_data(__mlibc_stack_data.argc, __mlibc_stack_data.argv,
			__mlibc_stack_data.envp);
}

extern "C" void __mlibc_entry() {
	char* ptr[2] = {"evilforest", nullptr};
	while (1) {}
	auto result = main(1, ptr, environ);
	exit(result);
}