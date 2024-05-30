// NT4.cpp : Defines the entry point for the application.
//



#include "NT4.h"
#include "include/HomeoStasis/c_Granulator.h"
#include "include/HomeoStasis/c_AE_Interface.h"
#include "include/c_Gaia.h"

using namespace std;











































                    

//Config.ssv - This file configures the network and sets the initial hyper-parameters. It handles the loading of any node files needed when using pretrained models.






//Input.ssv - This file holds the input data, each row an index

//Output.ssv - This file holds the output data, a table holding traces.

//This isn't for the engine itself, but for the testing of it. To remove in release version.
void generate_Test_Data()
{
    c_Sim Simboi;
    for (int cou_Index = 0; cou_Index < 100; cou_Index++)
    {
        Simboi.iterate();
        Simboi.output();
        Simboi.output_F("Sim_Test_Data.txt");
    }
}

int main(int argc, char** argv)
{
    run_Gaia();
    //runmap();
    return 1;

    //NT4::c_GPT_4 GPT;
    NT4::c_GPT_5 GPT;

    GPT.run();


    /*
    uint64_t tmp_Data[5];
    NT4::c_Construct_API tmp_NT4;
    tmp_NT4.register_Construct("Many_To_One", "Raw");
    for (int cou_Index = 0; cou_Index < 100; cou_Index++)
    {
        tmp_Data[0] = 100;
        tmp_Data[1] = rand() % 10000;
        tmp_Data[2] = rand() % 10000;
        tmp_Data[3] = rand() % 10000;
        tmp_Data[4] = rand() % 10000;

        tmp_NT4.set_Input_uint("Raw", 5, tmp_Data);
        tmp_NT4.encode("Raw");
    }
    for (int cou_Index = 0; cou_Index < 100000; cou_Index++)
    {
        tmp_Data[0] = rand() % 10000;
        tmp_Data[1] = rand() % 10000;
        tmp_Data[2] = rand() % 10000;
        tmp_Data[3] = rand() % 10000;
        tmp_Data[4] = rand() % 10000;

        tmp_NT4.set_Input_uint("Raw", 5, tmp_Data);
        tmp_NT4.encode("Raw");
    }
    std::cout << "\n Query...";
    while (1)
    {
        tmp_Data[0] = 100;

        tmp_NT4.set_Input_uint("Raw", 1, tmp_Data);
        tmp_NT4.reset_Output("Raw");
        tmp_NT4.query("Raw");
        tmp_NT4.gather_Treetops("Raw");
    }

    */


    std::string tmp_Autoexec = "../autoexec.ssv";
    if (argc > 1) { tmp_Autoexec = argv[1]; }
    NT4::c_Construct_Text_Server Serverman(tmp_Autoexec);






    //generate_Test_Data();
    //system("PAUSE");

    Serverman.run();

	return 0;
}
