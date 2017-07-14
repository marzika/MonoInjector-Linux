#include <iostream>
#include <link.h>
#include <fstream>

using namespace std;

typedef unsigned long DWORD;

typedef void* (*mono_thread_attach)(void* domain);
typedef void* (*mono_get_root_domain)();
typedef void* (*mono_assembly_open)(char* file, void* stat);
typedef void* (*mono_assembly_get_image)(void* assembly);
typedef void* (*mono_class_from_name)(void* image, char* namespacee, char* name);
typedef void* (*mono_class_get_method_from_name)(void* classs, char* name, DWORD param_count);
typedef void* (*mono_runtime_invoke)(void* method, void* instance, void* *params, void* exc);


mono_get_root_domain do_mono_get_root_domain;
mono_assembly_open do_mono_assembly_open;
mono_assembly_get_image do_mono_assembly_get_image;
mono_class_from_name do_mono_class_from_name;
mono_class_get_method_from_name do_mono_class_get_method_from_name;
mono_runtime_invoke do_mono_runtime_invoke;
mono_thread_attach do_mono_thread_attach;


inline uintptr_t GetAbsoluteAddress(uintptr_t instruction_ptr, int offset, int size)
{
    return instruction_ptr + *reinterpret_cast<uint32_t*>(instruction_ptr + offset) + size;
};

int __attribute__((constructor)) monoloaderinit()
{
    ofstream myfile ("loader.log");

    if (myfile.is_open())
    {
        myfile << "Initializing...\n";
        void* library = dlopen("./rust_Data/Mono/x86_64/libmono.so",  RTLD_NOLOAD | RTLD_NOW); //relative path to libmono in the game
        do_mono_thread_attach = reinterpret_cast<mono_thread_attach>(dlsym(library, "mono_thread_attach"));
        do_mono_get_root_domain = reinterpret_cast<mono_get_root_domain>(dlsym(library, "mono_get_root_domain"));


        do_mono_assembly_open = reinterpret_cast<mono_assembly_open>(dlsym(library, "mono_assembly_open"));
        do_mono_assembly_get_image = reinterpret_cast<mono_assembly_get_image>(dlsym(library, "mono_assembly_get_image"));
        do_mono_class_from_name = reinterpret_cast<mono_class_from_name>(dlsym(library, "mono_class_from_name"));
        do_mono_class_get_method_from_name = reinterpret_cast<mono_class_get_method_from_name>(dlsym(library, "mono_class_get_method_from_name"));

        do_mono_runtime_invoke = reinterpret_cast<mono_runtime_invoke>(dlsym(library, "mono_runtime_invoke"));

        myfile << "Started injection\n";

        // Init code //
        do_mono_thread_attach(do_mono_get_root_domain());
        myfile << "mono_get_root_domain\n";

        void* assembly = do_mono_assembly_open("PATH TO RustHax.dll", NULL);
        myfile << "mono_assembly_open\n";
        void* Image = do_mono_assembly_get_image(assembly);
        myfile << "mono_assembly_get_image\n";
        void* MonoClass = do_mono_class_from_name(Image, "RustHax", "Loader");
        myfile << "mono_class_from_name\n";
        void* MonoClassMethod = do_mono_class_get_method_from_name(MonoClass, "Load", 0);
        myfile << "mono_class_get_method_from_name\n";

        do_mono_runtime_invoke(MonoClassMethod, NULL, NULL, NULL);
        myfile << "mono_runtime_invoke\n";
        myfile << "Finished injection\n";
        myfile.close();
    }
    else cout << "Unable to open file";

    return 0;
}

void __attribute__((destructor)) monoloadershutdown()
{

};
