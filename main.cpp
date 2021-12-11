#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include "jpeg_source/jpegenc.cpp"
#include "jpeg_source/jpegdec.cpp"

using namespace std;


void encode(const string &input_file, const string &output_file) {
    // "Usage: %s <input.jpg> [<output.ppm>]\n", argv[0]

    const char *args[] = {
            "<this_program.exe>",
            input_file.c_str(),
            output_file.c_str(),
    };
    decode_main(3, args);

//    decode_main()
//
//
//    std::ifstream input(input_file, std::ios_base::binary);
//    if (!input.is_open()) {
//        std::cout << "Cannot open file for reading\n";
//        return 1;
//    }
//    std::ofstream output(output_file, std::ios_base::binary);
//    if (!output.is_open()) {
//        std::cout << "Cannot open file for writing\n";
//        return 1;
//    }
}

void decode(const string &input_file, const string &output_file) {


//    bool jo_write_jpg(const char *filename, const void *data, int width, int height, int comp, int quality);
    const char *args[] = {
            "<this_program.exe>",
            input_file.c_str(),
            output_file.c_str(),
            "490",
            "733",
            "3",
            "80"
    };
    if (!encode_main(7, args)) {
        printf("ERROR\n");
    }


//    std::ifstream input(input_file, std::ios_base::binary);
//    if (!input.is_open()) {
//        std::cout << "Cannot open file for reading\n";
//        return 1;
//    }
//    std::ofstream output(output_file, std::ios_base::binary);
//    if (!output.is_open()) {
//        std::cout << "Cannot open file for writing\n";
//        return 1;
//    }
//
//
//    printf("Usage: %s <input.jpg> [<output.ppm>]\n", argv[0]);
}


int main(int n, char* args[]) {

#ifdef DECODER
    const char *mode_str = "d";
#else
    const char *mode_str = "e";
#endif

    try {
        double start_time = clock();
        if (n != 3) {
            std::cout << "Wrong number of arguments\nUsage: \"program.exe <input filename> <output filename>\" \n";
            return 1;
        }
        int mode;

        if (strcmp(mode_str, "d") == 0) {
            mode = 1; // decoder
        } else {
            mode = 2; // encoder
        }

        string input_file = args[1];
        string output_file = args[2];

        if (mode == 1) {
            decode(input_file, output_file);
        } else if (mode == 2) {
            encode(input_file, output_file);
        }

        std::cout << "Done successfully, elapsed time: "
                << (static_cast<double>(clock()) - start_time) / static_cast<double>(CLOCKS_PER_SEC)
                << " seconds\n";

    } catch (const std::runtime_error &e){
        std::cout << e.what();
    }
    return 0;
}