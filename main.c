#include <stdlib.h>
#include "Tools.h"
#include "Config.h"
#include "Utils.h"
#include "IO.h"
#include "ToolsManager.h"


int main(int argc, char** argv) {
    char* FUNC = argv[1];
    char* processed_file_name;
    int last_opt_ind;
    Config config = parse_opts(argc, argv, &last_opt_ind);
    if(argc - 1 > last_opt_ind + 1) processed_file_name = argv[argc - 1];
    else{
        cleanup_config_allocation(&config);
        print_error("Вы забыли указать имя входного/выходного файла!");
        exit(1);
    }
    ProcessImage(FUNC, argv + 2,  argc - (last_opt_ind + 1) - 1, processed_file_name, config);
    cleanup_config_allocation(&config);
    print_success("Программа отработала успешно. Можете насладиться результатом!");
    return 0;
}
