

class c_Homeostasis_IO_Module
{
public:

    //The count.
    int Afferent_Count; //Variables dependent on this node: Afferent[]
    int Efferent_Count; //Variables dependent on this node: Efferent[]

    //One input for each index.
    c_AE_IO_Element** Afferent;
    c_AE_IO_Element** Efferent;

    //The array to hold the gathered input, and the count of elements in it.
    uint64_t* Gathered_Afferent[3]; //[0]: Concrete, [1]: Granulated, [2]: Delta
    uint64_t* Gathered_Efferent; //This is separate because it can have a different depth than the afferent one.

    std::vector<u_Data> Values;

    c_Homeostasis_IO_Module()
    {
        Afferent_Count = 0;
        Efferent_Count = 0;

        //Afferent = new c_AE_IO_Element * [1]; Afferent[0] = new c_AE_IO_Element;
        //Efferent = new c_AE_IO_Element * [1]; Efferent[0] = new c_AE_IO_Element;

        Afferent = NULL;
        Efferent = NULL;

        //For afferent and efferent 1 each. The depth of this is based on (Afferent_Count + Efferent_Count)
        Gathered_Afferent[0] = new uint64_t[1]; Gathered_Afferent[0][0] = 0;
        Gathered_Afferent[1] = new uint64_t[1]; Gathered_Afferent[1][0] = 0;
        Gathered_Afferent[2] = new uint64_t[1]; Gathered_Afferent[2][0] = 0;
        Gathered_Efferent = new uint64_t[1]; Gathered_Efferent[0] = 0;

        std::cout << "\n A: " << Afferent_Count << " E: " << Efferent_Count;
    }

    //Register Afferent input. The module always starts with 1.
    int register_Afferent()
    {
        std::cout << "\n register_Afferent(): ";

        //Declare the tmp arrays.
        c_AE_IO_Element** tmp_A;
        tmp_A = new c_AE_IO_Element * [Afferent_Count];

        //Swap the indices to the tmp array.
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            tmp_A[cou_A] = Afferent[cou_A]; Afferent[cou_A] = NULL;
        }

        //Delete and redeclare the real arrays.
        delete[] Afferent; Afferent = NULL;

        //Redeclare the OG arrays.
        Afferent = new c_AE_IO_Element * [Afferent_Count + 1];

        //create the new indices.
        Afferent[Afferent_Count] = new c_AE_IO_Element;

        //Recover the old data.
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            Afferent[cou_A] = tmp_A[cou_A]; tmp_A[cou_A] = NULL;
        }

        //Cleanup tmp vars
        //Delete and redeclare the real arrays.
        delete[] tmp_A; tmp_A = NULL;

        Afferent_Count++;

        std::cout << "\n register_Afferent(): " << (Afferent_Count - 1);

        return (Afferent_Count - 1);
    }

    //Register Efferent input/output.
    int register_Efferent()
    {
        //Declare the tmp arrays.
        c_AE_IO_Element** tmp_E;
        tmp_E = new c_AE_IO_Element * [Efferent_Count];

        //Swap the indices to the tmp array.
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            tmp_E[cou_E] = Efferent[cou_E]; Efferent[cou_E] = NULL;
        }

        //Delete and redeclare the real arrays.
        delete[] Efferent; Efferent = NULL;

        //Redeclare the OG arrays.
        Efferent = new c_AE_IO_Element * [Efferent_Count + 1];

        //NULL the new indices.
        Efferent[Efferent_Count] = new c_AE_IO_Element;

        //Recover the old data.
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            Efferent[cou_E] = tmp_E[cou_E]; tmp_E[cou_E] = NULL;
        }

        //Cleanup tmp vars
        //Delete and redeclare the real arrays.
        delete[] tmp_E; tmp_E = NULL;

        Efferent_Count++;

        std::cout << "\n register_Efferent(): " << (Efferent_Count - 1);

        return (Efferent_Count - 1);
    }

    //Accepts an input and updates the correct index with it
    void add_Afferent_Granulation(double p_Bottom, double p_Top, int p_Index = -1) //p_Index comes after since it may be 0
    {
        if (p_Index == -1)
        {
            p_Index = Afferent_Count - 1;
        }

        Afferent[p_Index]->add_Granulation(p_Bottom, p_Top);
    }

    //Exposes the set_Depth() so the user can access it through a function.
    //This extends the input depth of each index so that you can store short term memory of sorts.
    void set_Depth(int p_Depth)
    {
        //For every Afferent and Efferent we set the depth.
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            Afferent[cou_A]->set_Depth(p_Depth);
        }
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            Efferent[cou_E]->set_Depth(p_Depth);
        }
    }

    //Exposes the set_Value() so the user can access it through a function.
    //Assumes p_Index has been registered in the Afferent array already.
    void set_Afferent_Value(int p_Index, double p_Value)
    {
        Afferent[p_Index]->set_Value(p_Value);
    }

    //Exposes the set_Value() so the user can access it through a function.
    //Assumes p_Index has been registered in the Afferent array already.
    void set_Efferent_Value(int p_Index, double p_Value)
    {
        Efferent[p_Index]->set_Value(p_Value);
    }

    //Exposes the set_Value() so the user can access it through a function.
    void shift_Data()
    {
        //For every Afferent and Efferent we shift the data.
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            Afferent[cou_A]->shift_Data();
        }
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            Efferent[cou_E]->shift_Data();
        }
    }

    //This is done whenever an afferent or efferent is registered.
    void resize_Gathered_Input()
    {
        delete[] Gathered_Afferent[0]; Gathered_Afferent[0] = NULL;
        delete[] Gathered_Afferent[1]; Gathered_Afferent[1] = NULL;
        delete[] Gathered_Afferent[2]; Gathered_Afferent[2] = NULL;
        delete[] Gathered_Efferent; Gathered_Efferent = NULL;

        Gathered_Afferent[0] = new uint64_t[Afferent_Count];
        Gathered_Afferent[1] = new uint64_t[Afferent_Count];
        Gathered_Afferent[2] = new uint64_t[Afferent_Count];
        Gathered_Efferent = new uint64_t[Efferent_Count];

        for (int cou_Index = 0; cou_Index < Afferent_Count; cou_Index++)
        {
            Gathered_Afferent[0][cou_Index] = 0;
            Gathered_Afferent[1][cou_Index] = 0;
            Gathered_Afferent[2][cou_Index] = 0;
        }
        for (int cou_Index = 0; cou_Index < Efferent_Count; cou_Index++)
        {
            Gathered_Efferent[cou_Index] = 0;
        }
    }

    //Gathers the input set.
    void gather_Input()
    {
        //---std::cout << "\n\n gather_Input()";
        for (int cou_Index = 0; cou_Index < Afferent_Count; cou_Index++)
        {
            Gathered_Afferent[0][cou_Index] = Afferent[cou_Index]->get_Value_Data_uint64_t();
            Gathered_Afferent[1][cou_Index] = Afferent[cou_Index]->get_Value_Granulated_uint64_t();
            Gathered_Afferent[2][cou_Index] = Afferent[cou_Index]->get_Value_Delta_uint64_t();

            //---std::cout << "\n   a-[" << cou_Index << "]: C: " << Gathered_Afferent[0][cou_Index] << " G: " << Gathered_Afferent[1][cou_Index] << " D: " << Gathered_Afferent[2][cou_Index];
        }
        //---std::cout << "\n";
        for (int cou_Index = 0; cou_Index < Efferent_Count; cou_Index++)
        {
            Gathered_Efferent[cou_Index] = Efferent[cou_Index]->get_Value_Data_uint64_t();

            //---std::cout << "   e-[" << cou_Index << "]: V: " << Gathered_Efferent[cou_Index];
        }
    }

    //Exposes the output_IO() so the user can access it through a function.
    void output_AE()
    {
        //For every Afferent and Efferent we shift the data.
        std::cout << "\n ---Afferent_Count: " << Afferent_Count;
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            std::cout << "\n    [" << cou_A << "]";
            Afferent[cou_A]->output();
        }
        std::cout << "\n ---Efferent_Count: " << Efferent_Count;
        for (int cou_E = 0; cou_E < Efferent_Count; cou_E++)
        {
            std::cout << "\n    [" << cou_E << "]";
            Efferent[cou_E]->output();
        }
    }

    //Outputs the deviation mapping.
    void output_Deviation_Mapping()
    {
        std::cout << "\n ---Afferent_Count: " << Afferent_Count;
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            std::cout << "\n    [" << cou_A << "]";
            
            for (int cou_Chron = 0; cou_Chron < Afferent[cou_A]->Depth; cou_Chron++)
            {
                std::cout << "(" << Afferent[cou_A]->Deviation[cou_Chron] << ")";
            }
        }
    }

    //Outputs the deviation mapping.
    std::vector<float> get_Current_Deviation_Set()
    {
        std::vector<float> tmp_Return;
        tmp_Return.resize(Afferent_Count);

        //---std::cout << "\n ---Afferent_Count: " << Afferent_Count;
        for (int cou_A = 0; cou_A < Afferent_Count; cou_A++)
        {
            tmp_Return[cou_A] = Afferent[cou_A]->Deviation[Afferent[cou_A]->Depth - 1];
        }

        return tmp_Return;
    }

    float get_Current_Afferent_Deviation(int p_Afferent)
    {
        return Afferent[p_Afferent]->Deviation[Afferent[p_Afferent]->Depth - 1];
    }

    //Outputs the gathered Inputs
    void output_Gathered()
    {
        for (int cou_Index = 0; cou_Index < Afferent_Count; cou_Index++)
        {
            std::cout << "\n [0] " << Gathered_Afferent[0][cou_Index];
            std::cout << " [1] " << Gathered_Afferent[1][cou_Index];
            std::cout << " [2] " << Gathered_Afferent[2][cou_Index];
        }
        std::cout << "\n";
        for (int cou_Index = 0; cou_Index < Efferent_Count; cou_Index++)
        {
            std::cout << " {[" << cou_Index << "] " << Gathered_Efferent[cou_Index] << " } ";
        }
    }
};






























class c_Projection
{
public:

    std::vector<std::vector<std::vector<u_Data>>> Data;
};

//Each input in the homeostasis module input array is given a granulator that can be configured individually.
class c_Homeostasis_Module
{
public:

    c_Homeostasis_IO_Module IO;

    std::vector<c_Homeostasis_IO_Module> IO_Hist;

    c_Projection Current_Projection;

    std::vector<c_Projection> Projection_History;

    NT4::c_Time_Series_Generator_Module TSG;

    std::vector<int> Output_Signals;
    std::vector<int> No_Streak_Output_Signals;

    int No_Streak;
    int No_Streak_On;
    int No_Streak_Off;

    //Boredom mechanic.
    int Previous_Node_Count;

    c_Homeostasis_Module()
    {
        No_Streak = 0;
        No_Streak_On = 0;
        No_Streak_Off = 0;
        Previous_Node_Count = 0;
    }

    void init(int p_Chrono_Depth)
    {
        IO.set_Depth(p_Chrono_Depth);
        init_TSG(p_Chrono_Depth);
    }

    //Call after you've registered all your afferent and efferent.
    void init_TSG(int p_Chrono_Depth)
    {
        int tmp_IO_Depth = ((IO.Afferent_Count) * 3) + ((IO.Efferent_Count) * 3);

        TSG.init(p_Chrono_Depth, tmp_IO_Depth, 5);
    }

    //Encodes an input set.
    void encode()
    {
        TSG.encode(0);
    }

    void write_Bulk(std::string p_FName)
    {
        std::vector<std::vector<std::vector<NT4::s_Out>>> tmp_Bulk;

        tmp_Bulk = TSG.get_Bulk(1);


        std::ofstream BSF_D;
        std::ofstream BSF_M;
        std::ofstream BSF_C;
        std::ofstream BSF_R;
        std::string tmp_BName_D = "./GaiaTesting/bulk/" + p_FName + ".Primitive.blk";
        std::string tmp_BName_M = "./GaiaTesting/bulk/" + p_FName + ".Match.blk";
        std::string tmp_BName_C = "./GaiaTesting/bulk/" + p_FName + ".Charge.blk";
        std::string tmp_BName_R = "./GaiaTesting/bulk/" + p_FName + ".RC.blk";
        BSF_D.open(tmp_BName_D, std::ios::app);
        BSF_M.open(tmp_BName_M, std::ios::app);
        BSF_C.open(tmp_BName_C, std::ios::app);
        BSF_R.open(tmp_BName_R, std::ios::app);

        BSF_D << "\n\n";
        BSF_M << "\n\n";
        BSF_C << "\n\n";
        BSF_R << "\n\n";

        if (tmp_Bulk.size() == 0) { BSF_D << "0"; BSF_M << "0"; BSF_C << "0"; BSF_R << "0"; BSF_D.close();  BSF_M.close();  BSF_C.close();  BSF_R.close(); return; }
        if (tmp_Bulk[0].size() == 0) { BSF_D << "0"; BSF_M << "0"; BSF_C << "0"; BSF_R << "0"; BSF_D.close();  BSF_M.close();  BSF_C.close();  BSF_R.close(); return; }

        for (int cou_O = 0; cou_O < tmp_Bulk[0][0].size(); cou_O++)
        {
            BSF_D << "\n" << cou_O << " ";
            BSF_M << "\n" << cou_O << " ";
            BSF_C << "\n" << cou_O << " ";
            BSF_R << "\n" << cou_O << " ";
            for (int cou_Raw = 0; cou_Raw < tmp_Bulk[0].size(); cou_Raw++)
            {
                for (int cou_Chrono = 0; cou_Chrono < tmp_Bulk.size(); cou_Chrono++)
                {
                    BSF_D << " " << tmp_Bulk[cou_Chrono][cou_Raw][cou_O].Data.D;
                    BSF_M << " " << tmp_Bulk[cou_Chrono][cou_Raw][cou_O].Match.D;
                    BSF_C << " " << tmp_Bulk[cou_Chrono][cou_Raw][cou_O].Charge;
                    BSF_R << " " << tmp_Bulk[cou_Chrono][cou_Raw][cou_O].RC;
                }
            }
        }
        BSF_D.close();
        BSF_M.close();
        BSF_C.close();
        BSF_R.close();
    }

    void evaluate_Traces(std::string p_FName)
    {
        std::cout << "\n Trace Selection in progress...";
        std::vector<std::vector<std::vector<NT4::s_Out>>> tmp_Bulk;

        tmp_Bulk = TSG.get_Bulk(1);
        //write_Bulk(p_FName);

        // [-----]
        // [--+-+]
        // [+++++]
        //I need one of these evaluators for every trace.

        /*
        std::cout << "\n\n\n tmp_Bulk:";
        for (int cou_Chrono = 0; cou_Chrono < tmp_Bulk.size(); cou_Chrono++)
        {
            std::cout << "\n    Chrono[" << cou_Chrono << "]";

            for (int cou_Raw = ((IO.Afferent_Count - 1) * 3); cou_Raw < tmp_Bulk[cou_Chrono].size(); cou_Raw++)
            //for (int cou_Raw = 0; cou_Raw < tmp_Bulk[cou_Chrono].size(); cou_Raw++)
            {
                std::cout << "\n       Raw[" << cou_Raw << "]";
                for (int cou_O = 0; cou_O < tmp_Bulk[cou_Chrono][cou_Raw].size(); cou_O++)
                {
                    std::cout << "\n          O[" << cou_O << "]";
                    std::cout << " Bulk_Primitive: " << tmp_Bulk[cou_Chrono][cou_Raw][cou_O].Data.D;
                    std::cout << " Match: " << tmp_Bulk[cou_Chrono][cou_Raw][cou_O].Match.D;
                    std::cout << " Charge: " << tmp_Bulk[cou_Chrono][cou_Raw][cou_O].Charge;
                }
            }
        }*/


        //---std::cout << "\n\n _~_ tmp_Bulk Checker:";

        float tmp_Count = 0;

        int tmp_Raw_Depth = 0;
        int tmp_Chrono_Depth = 0;
        int tmp_Output_Depth = 0;

        std::vector<std::vector<int>> tmp_Validate_Start_Anchor;
        std::vector<std::vector<std::vector<int>>> tmp_Validate_Direction;
        std::vector<std::vector<std::vector<int>>> tmp_Validate_VDirection;
        std::vector<std::vector<std::vector<float>>> tmp_Validate_Signals;
        std::vector<std::vector<float>> tmp_Validate_Signals_Sum;
        std::vector<int> tmp_Validate_Start_Anchor_Sum;
        std::vector<int> tmp_Validate_Direction_Sum;
        std::vector<std::vector<float>> tmp_Validate_RC;
        std::vector<float> tmp_Validate_RC_Sum;
        std::vector<float> tmp_Validate_Charge;

        float tmp_Normalized_Start_Anchor_Sum = 0;
        float tmp_Normalized_Direction_Sum = 0;
        float tmp_Normalized_RC_Sum = 0;
        float tmp_Normalized_Charge_Sum = 0;

        std::vector<std::vector<float>> tmp_Output_Signals;

        bool flg_Bored = false;

        if (tmp_Bulk.size() > 0)
        {
            tmp_Chrono_Depth = tmp_Bulk.size();

            if (tmp_Chrono_Depth > 0)
            {
                tmp_Raw_Depth = tmp_Bulk[0].size();

                if (tmp_Raw_Depth > 0)
                {
                    tmp_Output_Depth = tmp_Bulk[0][0].size();
                }
            }
        }


        tmp_Validate_Direction.resize(tmp_Chrono_Depth);
        tmp_Validate_VDirection.resize(tmp_Chrono_Depth);

        tmp_Validate_Signals.resize(tmp_Chrono_Depth);
        tmp_Validate_Signals_Sum.resize(tmp_Chrono_Depth);

        tmp_Validate_Start_Anchor.resize(tmp_Raw_Depth);
        tmp_Validate_RC.resize(tmp_Raw_Depth);

        tmp_Output_Signals.resize(IO.Efferent_Count);

        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            //[0]Composite [1]SAS [2]DS [3]RC [4]Chrg
            tmp_Output_Signals[cou_E].resize(5);
        }

        for (int cou_Chrono = 0; cou_Chrono < tmp_Chrono_Depth; cou_Chrono++)
        {
            tmp_Validate_Direction[cou_Chrono].resize(tmp_Raw_Depth);
            tmp_Validate_VDirection[cou_Chrono].resize(tmp_Raw_Depth);

            for (int cou_R = 0; cou_R < tmp_Raw_Depth; cou_R++)
            {
                tmp_Validate_Start_Anchor[cou_R].resize(tmp_Output_Depth);
                tmp_Validate_Direction[cou_Chrono][cou_R].resize(tmp_Output_Depth);
                tmp_Validate_VDirection[cou_Chrono][cou_R].resize(tmp_Output_Depth);
                tmp_Validate_RC[cou_R].resize(tmp_Output_Depth);
            }

            tmp_Validate_Signals[cou_Chrono].resize(IO.Efferent_Count);
            tmp_Validate_Signals_Sum[cou_Chrono].resize(IO.Efferent_Count);

            for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
            {
                tmp_Validate_Signals[cou_Chrono][cou_E].resize(tmp_Output_Depth);
            }
        }

        tmp_Validate_Start_Anchor_Sum.resize(tmp_Output_Depth);
        tmp_Validate_Direction_Sum.resize(tmp_Output_Depth);
        tmp_Validate_Charge.resize(tmp_Output_Depth);
        tmp_Validate_RC_Sum.resize(tmp_Output_Depth);

        //TSG.output_Bulk(1);

        //TSG.output_IO(0);

        if ((tmp_Chrono_Depth > 0) && (tmp_Raw_Depth > 0))
        {
            for (int cou_O = 0; cou_O < tmp_Output_Depth; cou_O++)
            {
                tmp_Validate_Charge[cou_O] = tmp_Bulk[0][0][cou_O].Charge;
            }
        }

        //For afferent count find the matches to the starting value
        //---std::cout << "\n Evalitor:";
        for (int cou_A = 0; cou_A < IO.Afferent_Count; cou_A++)
        {
            //---std::cout << "\n ___ A[" << cou_A << "]";
            for (int cou_O = 0; cou_O < tmp_Output_Depth; cou_O++)
            {
                //---std::cout << "\n ___ ___ O[" << cou_O << "]";
                int tmp_AIndex = (cou_A * 3);

                tmp_Bulk[0][(cou_A * 3)][cou_O].flg_Use = 0;

                //---std::cout << " - AIndex[" << tmp_AIndex << "]";
                //---std::cout << "  D: " << tmp_Bulk[0][tmp_AIndex][cou_O].Data.D;
                //---std::cout << "  I: " << TSG.get_Input(0, 0, tmp_AIndex);
                //---std::cout << "  ?= " << (tmp_Bulk[0][tmp_AIndex][cou_O].Data.D == TSG.get_Input(0, 0, tmp_AIndex));
                if (tmp_Bulk[0][tmp_AIndex][cou_O].Data.D == TSG.get_Input(0, 0, tmp_AIndex))
                {
                    tmp_Bulk[0][tmp_AIndex][cou_O].flg_Use = 1;
                    tmp_Validate_Start_Anchor[tmp_AIndex][cou_O] = 1;
                    tmp_Validate_Start_Anchor_Sum[cou_O]++;
                }
                tmp_Validate_RC[tmp_AIndex][cou_O] = tmp_Bulk[0][tmp_AIndex][cou_O].RC;
                tmp_Validate_RC_Sum[cou_O] += tmp_Bulk[0][tmp_AIndex][cou_O].RC;

                tmp_AIndex = ((cou_A * 3) + 1);

                //---std::cout << " - AIndex[" << tmp_AIndex << "]";
                //---std::cout << "  D: " << tmp_Bulk[0][tmp_AIndex][cou_O].Data.D;
                //---std::cout << "  I: " << TSG.get_Input(0, 0, tmp_AIndex);
                //---std::cout << "  ?= " << (tmp_Bulk[0][tmp_AIndex][cou_O].Data.D == TSG.get_Input(0, 0, tmp_AIndex));
                float tmp_D = tmp_Bulk[0][tmp_AIndex][cou_O].Data.D;
                float tmp_I = TSG.get_Input(0, 0, tmp_AIndex);
                //float tmp_I_L = (TSG.get_Input(0, 0, tmp_AIndex) - 1);
                //float tmp_I_H = (TSG.get_Input(0, 0, tmp_AIndex) + 1);
                //if ((tmp_D > tmp_I_L) && (tmp_D < tmp_I_H))
                if (tmp_D == tmp_I)
                {
                    tmp_Bulk[0][tmp_AIndex][cou_O].flg_Use = 1;
                    tmp_Validate_Start_Anchor[tmp_AIndex][cou_O] = 1;
                    tmp_Validate_Start_Anchor_Sum[cou_O]++;
                }

                tmp_Validate_RC[tmp_AIndex][cou_O] = tmp_Bulk[0][tmp_AIndex][cou_O].RC;
                tmp_Validate_RC_Sum[cou_O] += tmp_Bulk[0][tmp_AIndex][cou_O].RC;

                for (int cou_Chrono = 1; cou_Chrono < (tmp_Chrono_Depth); cou_Chrono++)
                {

                    tmp_AIndex = ((cou_A * 3) + 2);

                    //---std::cout << " - Chrono[" << cou_Chrono << "]";
                    //---std::cout << "  D: " << tmp_Bulk[cou_Chrono][tmp_AIndex][cou_O].Data.D;
                    //---std::cout << "  M: " << IO.get_Current_Afferent_Deviation(cou_A);
                    //---std::cout << "  ?= " << (tmp_Bulk[cou_Chrono][tmp_AIndex][cou_O].Data.D == IO.get_Current_Afferent_Deviation(cou_A));
                    if (tmp_Bulk[cou_Chrono][tmp_AIndex][cou_O].Data.D == IO.get_Current_Afferent_Deviation(cou_A))
                    {
                        tmp_Validate_Direction[cou_Chrono][cou_A][cou_O]++;
                        tmp_Validate_Direction_Sum[cou_O]++;
                    }
                    tmp_Validate_RC[tmp_AIndex][cou_O] = tmp_Bulk[0][tmp_AIndex][cou_O].RC;
                    tmp_Validate_RC_Sum[cou_O] += tmp_Bulk[0][tmp_AIndex][cou_O].RC;
                }

                int flg_Good_Signal = 0;
                for (int cou_Chrono = (tmp_Chrono_Depth - 1); cou_Chrono >= 0; cou_Chrono--)
                {
                    if (tmp_Validate_Direction[cou_Chrono][cou_A][cou_O] > 0) { flg_Good_Signal = 1; }

                    tmp_Validate_VDirection[cou_Chrono][cou_A][cou_O] += flg_Good_Signal;
                }
            }
        }

        //Loop through each output trace, then loop through each efferent, loop through each index, if the index is >0 then check the VDirection for every afferent and ++ to the signal if it checks out.
        for (int cou_O = 0; cou_O < tmp_Output_Depth; cou_O++)
        {
            for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
            {
                //Get the Efferent index for the concrete data.
                int tmp_EIndex = (IO.Efferent_Count * 3) + (cou_E * 3);

                for (int cou_Chrono = 0; cou_Chrono < (tmp_Chrono_Depth - 1); cou_Chrono++)
                {
                    bool flg_New = 1;

                    //Checks if the signal is new or already on, edge detection.
                    if (cou_Chrono > 0)
                    {
                        flg_New = !(tmp_Bulk[cou_Chrono - 1][tmp_EIndex][cou_O].Data.D > 0);
                    }

                    if ((tmp_Bulk[cou_Chrono][tmp_EIndex][cou_O].Data.D > 0) && flg_New)
                    {
                        float tmp_Valid_Sig = 0;
                        for (int cou_A = 0; cou_A < IO.Afferent_Count; cou_A++)
                        {
                            for (int cou_F = (cou_Chrono + 1); cou_F < tmp_Chrono_Depth; cou_F++)
                            {
                                if (tmp_Validate_VDirection[cou_F][cou_A][cou_O] > 0)
                                {
                                    tmp_Valid_Sig++;
                                }
                                else
                                {
                                    tmp_Valid_Sig = 0;
                                }
                            }
                            /*
                            if (tmp_Validate_VDirection[cou_Chrono + 1][cou_A][cou_O] > 0)
                            {
                                tmp_Validate_Signals[cou_Chrono][cou_E][cou_O] += 1;
                            }
                            */
                        }

                        tmp_Validate_Signals[cou_Chrono][cou_E][cou_O] = tmp_Valid_Sig;
                    }
                }
            }
        }

        tmp_Normalized_Start_Anchor_Sum = 0;
        for (int cou_O = 0; cou_O < tmp_Output_Depth; cou_O++)
        {
            if (tmp_Normalized_Start_Anchor_Sum < tmp_Validate_Start_Anchor_Sum[cou_O]) { tmp_Normalized_Start_Anchor_Sum = tmp_Validate_Start_Anchor_Sum[cou_O]; }
            if (tmp_Normalized_Direction_Sum < tmp_Validate_Direction_Sum[cou_O]) { tmp_Normalized_Direction_Sum = tmp_Validate_Direction_Sum[cou_O]; }
            if (tmp_Normalized_RC_Sum < tmp_Validate_RC_Sum[cou_O]) { tmp_Normalized_RC_Sum = tmp_Validate_RC_Sum[cou_O]; }
            if (tmp_Normalized_Charge_Sum < tmp_Validate_Charge[cou_O]) { tmp_Normalized_Charge_Sum = tmp_Validate_Charge[cou_O]; }
        }

        if (tmp_Normalized_Start_Anchor_Sum > 0)
        {
            for (int cou_O = 0; cou_O < tmp_Output_Depth; cou_O++)
            {
                tmp_Validate_Start_Anchor_Sum[cou_O] = int((tmp_Validate_Start_Anchor_Sum[cou_O] / tmp_Normalized_Start_Anchor_Sum) * 100);
                tmp_Validate_Direction_Sum[cou_O] = int((tmp_Validate_Direction_Sum[cou_O] / tmp_Normalized_Direction_Sum) * 100);
                tmp_Validate_RC_Sum[cou_O] = int((tmp_Validate_RC_Sum[cou_O] / tmp_Normalized_RC_Sum) * 10);
                tmp_Validate_Charge[cou_O] = int((tmp_Validate_Charge[cou_O] / tmp_Normalized_Charge_Sum) * 10);
            }
        }
        else
        {
            for (int cou_O = 0; cou_O < tmp_Output_Depth; cou_O++)
            {
                tmp_Validate_Start_Anchor_Sum[cou_O] = 0;
                tmp_Validate_Direction_Sum[cou_O] = 0;
                tmp_Validate_RC_Sum[cou_O] = 0;
                tmp_Validate_Charge[cou_O] = 0;
            }

            //Select a random trace if there are any and set the scores to 1 to try it and see what happens.
            if (tmp_Output_Depth > 0)
            {
                int tmp_O = rand() % tmp_Output_Depth;

                tmp_Validate_Start_Anchor_Sum[tmp_O] = 1;
                tmp_Validate_Direction_Sum[tmp_O] = 1;
                tmp_Validate_RC_Sum[tmp_O] = 1;
                tmp_Validate_Charge[tmp_O] = 1;
            }
        }

        int tmp_Valid_Traces = 0;

        std::cout << "\n\n Trace Selection Scores:";
        for (int cou_O = 0; cou_O < tmp_Output_Depth; cou_O++)
        {
            bool tmp_Flg_Valid_Trace = false;

            if (tmp_Validate_Direction_Sum[cou_O] == 0) { continue; }
            if (tmp_Validate_Start_Anchor_Sum[cou_O] == 0) { continue; }

            //---std::cout << "\n[" << cou_O << "] ";
            //---std::cout << " DMatch ";
            for (int cou_A = 0; cou_A < IO.Afferent_Count; cou_A++)
            {
                
                /*
                std::cout << " " << cou_A << "[";
                for (int cou_Chrono = 1; cou_Chrono < (tmp_Chrono_Depth); cou_Chrono++)
                {
                    if (tmp_Validate_Direction[cou_Chrono][cou_A][cou_O] == 1)
                    {
                        std::cout << "+";
                    }
                    else
                    {
                        std::cout << " ";
                    }
                }
                std::cout << "] ";
                */
                //---std::cout << " " << cou_A << " [";
                for (int cou_Chrono = 1; cou_Chrono < (tmp_Chrono_Depth); cou_Chrono++)
                {
                    if (tmp_Validate_VDirection[cou_Chrono][cou_A][cou_O] == 1)
                    {
                        //---std::cout << "+";
                    }
                    else
                    {
                        //---std::cout << " ";
                    }
                }
                //---std::cout << "] ";
                
            }

            //---std::cout << " ... Sig ";

            
            for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
            {
                /*
                std::cout << " " << cou_E << "[";
                for (int cou_Chrono = 0; cou_Chrono < tmp_Chrono_Depth; cou_Chrono++)
                {
                    int tmp_Index = (IO.Afferent_Count * 3) + (cou_E * 3);
                    if (tmp_Bulk[cou_Chrono][tmp_Index][cou_O].Data.D == 1)
                    {
                        std::cout << "+";

                        //[0]Composite [1]SAS [2]DS [3]RC [4]Chrg
                        tmp_Output_Signals[cou_E][1] += tmp_Validate_Start_Anchor_Sum[cou_O];
                        tmp_Output_Signals[cou_E][2] += tmp_Validate_Direction_Sum[cou_O];
                        tmp_Output_Signals[cou_E][3] += tmp_Validate_RC_Sum[cou_O];
                        tmp_Output_Signals[cou_E][4] += tmp_Validate_Charge[cou_O];
                    }

                    if (tmp_Bulk[cou_Chrono][tmp_Index][cou_O].Data.D == -1)
                    {
                        std::cout << " ";
                    }

                    if (tmp_Bulk[cou_Chrono][tmp_Index][cou_O].Data.D == 0)
                    {
                        std::cout << " ";
                    }
                }
                std::cout << "]";
                */



                //---std::cout << " " << cou_E << "[";
                for (int cou_Chrono = 0; cou_Chrono < tmp_Chrono_Depth; cou_Chrono++)
                {
                    if (tmp_Validate_Signals[cou_Chrono][cou_E][cou_O] > 0)
                    {
                        //std::cout << "+";
                        //std::cout << char(tmp_Validate_Signals[cou_Chrono][cou_E][cou_O]);

                        //[0]Composite [1]SAS [2]DS [3]RC [4]Chrg
                        tmp_Output_Signals[cou_E][1] += tmp_Validate_Start_Anchor_Sum[cou_O];
                        tmp_Output_Signals[cou_E][2] += tmp_Validate_Direction_Sum[cou_O];
                        tmp_Output_Signals[cou_E][3] += tmp_Validate_RC_Sum[cou_O];
                        tmp_Output_Signals[cou_E][4] += tmp_Validate_Charge[cou_O];

                        tmp_Flg_Valid_Trace = true;
                    }

                    //tmp_Validate_Signals_Sum[cou_Chrono][cou_E] += tmp_Validate_Signals[cou_Chrono][cou_E][cou_O];

                    //---opchr(char(tmp_Validate_Signals[cou_Chrono][cou_E][cou_O]));
                }
                //---std::cout << "]";

                //---std::cout << " " << cou_E << "[";
                for (int cou_Chrono = 0; cou_Chrono < tmp_Chrono_Depth; cou_Chrono++)
                {
                    //---opchr(char(tmp_Validate_Signals_Sum[cou_Chrono][cou_E]));
                }
                //---std::cout << "]";
            }

            if (tmp_Flg_Valid_Trace == true)
            {
                tmp_Valid_Traces++;
            }

            //---std::cout << "\tSA: " << tmp_Validate_Start_Anchor_Sum[cou_O];
            //---std::cout << "\tDS: " << tmp_Validate_Direction_Sum[cou_O];
            //---std::cout << "\tRC: " << tmp_Validate_RC_Sum[cou_O];
            //---std::cout << "\tChrg: " << tmp_Validate_Charge[cou_O];
        }

        int tmp_Almost_Valid_Traces = 0;

        //If no traces try again without the limiter of the starting fixation.
        if (tmp_Valid_Traces == 0)
        {
            std::cout << "\n\n Trace Selection Scores - Start Anchor Limit Removed:";

            for (int cou_O = 0; cou_O < tmp_Output_Depth; cou_O++)
            {
                bool tmp_Flg_Valid_Trace = false;

                if (tmp_Validate_Direction_Sum[cou_O] == 0) { continue; }

                //---std::cout << "\n[" << cou_O << "] ";
                //---std::cout << " DMatch ";
                for (int cou_A = 0; cou_A < IO.Afferent_Count; cou_A++)
                {
                    //---std::cout << " " << cou_A << " [";
                    for (int cou_Chrono = 1; cou_Chrono < (tmp_Chrono_Depth); cou_Chrono++)
                    {
                        if (tmp_Validate_VDirection[cou_Chrono][cou_A][cou_O] == 1)
                        {
                            //---std::cout << "+";
                        }
                        else
                        {
                            //---std::cout << " ";
                        }
                    }
                    //---std::cout << "] ";

                }

                //---std::cout << " ... Sig ";


                for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
                {

                    //---std::cout << " " << cou_E << "[";
                    for (int cou_Chrono = 0; cou_Chrono < tmp_Chrono_Depth; cou_Chrono++)
                    {
                        if (tmp_Validate_Signals[cou_Chrono][cou_E][cou_O] > 0)
                        {
                            //std::cout << "+";
                            //std::cout << char(tmp_Validate_Signals[cou_Chrono][cou_E][cou_O]);

                            //[0]Composite [1]SAS [2]DS [3]RC [4]Chrg
                            tmp_Output_Signals[cou_E][1] += tmp_Validate_Start_Anchor_Sum[cou_O];
                            tmp_Output_Signals[cou_E][2] += tmp_Validate_Direction_Sum[cou_O];
                            tmp_Output_Signals[cou_E][3] += tmp_Validate_RC_Sum[cou_O];
                            tmp_Output_Signals[cou_E][4] += tmp_Validate_Charge[cou_O];

                            tmp_Flg_Valid_Trace = true;
                        }

                        //tmp_Validate_Signals_Sum[cou_Chrono][cou_E] += tmp_Validate_Signals[cou_Chrono][cou_E][cou_O];

                        opchr(char(tmp_Validate_Signals[cou_Chrono][cou_E][cou_O]));
                    }
                    //---std::cout << "]";

                    //---std::cout << " " << cou_E << "[";
                    for (int cou_Chrono = 0; cou_Chrono < tmp_Chrono_Depth; cou_Chrono++)
                    {
                        //---opchr(char(tmp_Validate_Signals_Sum[cou_Chrono][cou_E]));
                    }
                    //---std::cout << "]";
                }


                if (tmp_Flg_Valid_Trace == true)
                {
                    tmp_Almost_Valid_Traces++;
                }

            }

        }

        std::vector<float> tmp_Highest_Output_Signal;
        tmp_Highest_Output_Signal.resize(5);

        std::vector<float> tmp_Lowest_Output_Signal;
        tmp_Lowest_Output_Signal.resize(5);

        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            if (tmp_Highest_Output_Signal[1] < tmp_Output_Signals[cou_E][1]) { tmp_Highest_Output_Signal[1] = tmp_Output_Signals[cou_E][1]; }
            if (tmp_Highest_Output_Signal[2] < tmp_Output_Signals[cou_E][2]) { tmp_Highest_Output_Signal[2] = tmp_Output_Signals[cou_E][2]; }
            if (tmp_Highest_Output_Signal[3] < tmp_Output_Signals[cou_E][3]) { tmp_Highest_Output_Signal[3] = tmp_Output_Signals[cou_E][3]; }
            if (tmp_Highest_Output_Signal[4] < tmp_Output_Signals[cou_E][4]) { tmp_Highest_Output_Signal[4] = tmp_Output_Signals[cou_E][4]; }
        }


        /*
        tmp_Lowest_Output_Signal[0] = tmp_Highest_Output_Signal[0];
        tmp_Lowest_Output_Signal[1] = tmp_Highest_Output_Signal[1];
        tmp_Lowest_Output_Signal[2] = tmp_Highest_Output_Signal[2];
        tmp_Lowest_Output_Signal[3] = tmp_Highest_Output_Signal[3];
        tmp_Lowest_Output_Signal[4] = tmp_Highest_Output_Signal[4];

        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            if (tmp_Lowest_Output_Signal[1] > tmp_Output_Signals[cou_E][1]) { tmp_Lowest_Output_Signal[1] = tmp_Output_Signals[cou_E][1]; }
            if (tmp_Lowest_Output_Signal[2] > tmp_Output_Signals[cou_E][2]) { tmp_Lowest_Output_Signal[2] = tmp_Output_Signals[cou_E][2]; }
            if (tmp_Lowest_Output_Signal[3] > tmp_Output_Signals[cou_E][3]) { tmp_Lowest_Output_Signal[3] = tmp_Output_Signals[cou_E][3]; }
            if (tmp_Lowest_Output_Signal[4] > tmp_Output_Signals[cou_E][4]) { tmp_Lowest_Output_Signal[4] = tmp_Output_Signals[cou_E][4]; }
        }

        tmp_Highest_Output_Signal[0] -= tmp_Lowest_Output_Signal[0];
        tmp_Highest_Output_Signal[1] -= tmp_Lowest_Output_Signal[1];
        tmp_Highest_Output_Signal[2] -= tmp_Lowest_Output_Signal[2];
        tmp_Highest_Output_Signal[3] -= tmp_Lowest_Output_Signal[3];
        tmp_Highest_Output_Signal[4] -= tmp_Lowest_Output_Signal[4];
        */


        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            //tmp_Output_Signals[cou_E][1] = (tmp_Output_Signals[cou_E][1] - tmp_Lowest_Output_Signal[1]) / tmp_Highest_Output_Signal[1];
            //tmp_Output_Signals[cou_E][2] = (tmp_Output_Signals[cou_E][2] - tmp_Lowest_Output_Signal[2]) / tmp_Highest_Output_Signal[2];
            //tmp_Output_Signals[cou_E][3] = (tmp_Output_Signals[cou_E][3] - tmp_Lowest_Output_Signal[3]) / tmp_Highest_Output_Signal[3];
            //tmp_Output_Signals[cou_E][4] = (tmp_Output_Signals[cou_E][4] - tmp_Lowest_Output_Signal[4]) / tmp_Highest_Output_Signal[4];
            //tmp_Output_Signals[cou_E][1] = (tmp_Output_Signals[cou_E][1] / tmp_Highest_Output_Signal[1]) * 10;
            //tmp_Output_Signals[cou_E][2] = (tmp_Output_Signals[cou_E][2] / tmp_Highest_Output_Signal[2]) * 2;
            tmp_Output_Signals[cou_E][3] = (tmp_Output_Signals[cou_E][3] / tmp_Highest_Output_Signal[3]) * 1;
            tmp_Output_Signals[cou_E][4] = (tmp_Output_Signals[cou_E][4] / tmp_Highest_Output_Signal[4]) * 1;

            for (int cou_Rawr = 1; cou_Rawr < 5; cou_Rawr++)
            {
                tmp_Output_Signals[cou_E][0] += tmp_Output_Signals[cou_E][cou_Rawr];
            }
            
            for (int cou_Chrono = 0; cou_Chrono < tmp_Chrono_Depth; cou_Chrono++)
            {
                //tmp_Output_Signals[cou_E][0] += tmp_Validate_Signals_Sum[cou_Chrono][cou_E];
            }

            std::cout << "\nOutput_Signal[" << cou_E << "] ";
            std::cout << "Fn: " << int(tmp_Output_Signals[cou_E][0] * 100) << "      ";
            std::cout << "\tSA: " << int(tmp_Output_Signals[cou_E][1] * 10);
            std::cout << "\tDM: " << int(tmp_Output_Signals[cou_E][2] * 10);
            std::cout << "\tRC: " << int(tmp_Output_Signals[cou_E][3] * 10);
            std::cout << "\tChrg: " << int(tmp_Output_Signals[cou_E][4] * 10);
        }

        std::cout << "\n\n\t Valid Traces Found: " << tmp_Valid_Traces;

        std::ofstream tmp_OF;
        std::string tmp_FName = "./GaiaTesting/" + p_FName + ".Valid_Traces.ssv";
        tmp_OF.open(tmp_FName, std::ios::app);
        tmp_OF << tmp_Valid_Traces << "\n";
        tmp_OF.close();
        
        std::ofstream tmp_AOF;
        tmp_FName = "./GaiaTesting/" + p_FName + ".Nearly_Valid_Traces.ssv";
        tmp_AOF.open(tmp_FName, std::ios::app);
        tmp_AOF << tmp_Almost_Valid_Traces << "\n";
        tmp_AOF.close();

        std::ofstream tmp_NOF;
        tmp_FName = "./GaiaTesting/" + p_FName + ".Node_Count.ssv";
        tmp_NOF.open(tmp_FName, std::ios::app);
        tmp_NOF << TSG.NT4_Core.Base.Nodes.Node_Count << "\n";
        tmp_NOF.close();

        if ((Previous_Node_Count - TSG.NT4_Core.Base.Nodes.Node_Count) == 0)
        {
            flg_Bored = true;
        }

        Previous_Node_Count = TSG.NT4_Core.Base.Nodes.Node_Count;

        std::ofstream tmp_OTOF;
        tmp_FName = "./GaiaTesting/" + p_FName + ".Total_Output_Traces.ssv";
        tmp_OTOF.open(tmp_FName, std::ios::app);
        tmp_OTOF << tmp_Output_Depth << "\n";
        tmp_OTOF.close();
        

        float tmp_High_Fn = 0;
        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            if (tmp_High_Fn < tmp_Output_Signals[cou_E][0]) { tmp_High_Fn = tmp_Output_Signals[cou_E][0]; }
        }
        float tmp_Low_Fn = tmp_High_Fn;
        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            if (tmp_Low_Fn > tmp_Output_Signals[cou_E][0]) { tmp_Low_Fn = tmp_Output_Signals[cou_E][0]; }
        }

        tmp_High_Fn -= tmp_Low_Fn;

        if (tmp_High_Fn == 0) { tmp_High_Fn = tmp_Low_Fn; tmp_Low_Fn = 0; }

        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            tmp_Output_Signals[cou_E][0] -= tmp_Low_Fn;

            tmp_Output_Signals[cou_E][0] /= tmp_High_Fn;
        }

        tmp_High_Fn = 0;
        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            if (tmp_High_Fn < tmp_Output_Signals[cou_E][0]) { tmp_High_Fn = tmp_Output_Signals[cou_E][0]; }
        }

        Output_Signals.resize(IO.Efferent_Count);

        tmp_High_Fn *= 0.9;

        if (tmp_High_Fn == 0) { tmp_High_Fn = 0.1; }

        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            if (tmp_Output_Signals[cou_E][0] > tmp_High_Fn)
            {
                tmp_Output_Signals[cou_E][0] = 1;
                Output_Signals[cou_E] = 1;
            }
            else
            {
                tmp_Output_Signals[cou_E][0] = 0;
                Output_Signals[cou_E] = -1;
            }
        }

        int tmp_No_Streak_ONOFF = -1;
        if (tmp_Valid_Traces < 250)
        {
            if (No_Streak == 0)
            {
                No_Streak_Output_Signals.resize(IO.Efferent_Count);

                for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
                {
                    No_Streak_Output_Signals[cou_E] = -1;
                }

                //Pick one at random to try.
                for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
                {
                    if ((rand() % 2) == 0)
                    {
                        No_Streak_Output_Signals[cou_E] = 1;
                    }
                }

                No_Streak_On = rand() % IO.Efferent_Count;
                No_Streak_Off = rand() % IO.Efferent_Count;
            }


            No_Streak++;

            if (No_Streak >= (get_Chrono_Depth() * 5))
            {
                No_Streak = 0;
            }

            if (No_Streak > (get_Chrono_Depth() * 3))
            {
                tmp_No_Streak_ONOFF = 1;
            }
        }
        else
        {
            No_Streak = 0;
        }

        std::cout << "\n Afferent_Condition:";
        for (int cou_A = 0; cou_A < IO.Afferent_Count; cou_A++)
        {
            std::cout << "\n A[" << cou_A << "]: ";

            if (IO.get_Current_Afferent_Deviation(cou_A) > 0)
            {
                std::cout << " D[[LOW]______]";
            }
            else if (IO.get_Current_Afferent_Deviation(cou_A) < 0)
            {
                std::cout << " D[_____[HIGH]]";
            }
            else
            {
                std::cout << " D[___________]";
            }
        }
        std::cout << "\n";

        /*
        std::cout << "\n Random_Flailing: ";
        if (tmp_No_Streak_ONOFF == 1)
        { 
            std::cout << "[ON]-----"; 
        } 
        else 
        { 
            std::cout << "----[OFF]"; 
        }

        std::cout << " Current: " << No_Streak << " / " << (get_Chrono_Depth() * 5);

        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            std::cout << "\n E[" << cou_E << "]: ";

            if (tmp_No_Streak_ONOFF == 1)
            {
                if (No_Streak_Output_Signals[cou_E] == 1)
                {
                    std::cout << " NS[[ON]-----]";
                }
                else if (No_Streak_Output_Signals[cou_E] == -1)
                {
                    std::cout << " NS[----[OFF]]";
                }
                else
                {
                    std::cout << " NS[---------]";
                }
            }
            else
            {
                std::cout << " NS[---------]";
            }

            if (Output_Signals[cou_E] == 1)
            {
                std::cout << " S[[ON]-----]";
            }
            else if (Output_Signals[cou_E] == -1)
            {
                std::cout << " S[----[OFF]]";
            }
            else
            {
                std::cout << " S[---------]";
            }
        }
        */


        if (tmp_No_Streak_ONOFF == 1)
        {
            //Output_Signals[No_Streak_Off] = -1;
            //Output_Signals[No_Streak_On] = 1;

            for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
            {
                //Output_Signals[cou_E] = No_Streak_Output_Signals[cou_E];
            }
        }

        if (tmp_Valid_Traces <= 0)
        {
            int tmp_RanRan_Flail = rand() % IO.Efferent_Count;

            Output_Signals[tmp_RanRan_Flail] = 1;
        }

        if (flg_Bored)
        {
            if ((rand() % 5) == 0)
            {
                int tmp_RanRan_Flail = rand() % IO.Efferent_Count;

                //Output_Signals[tmp_RanRan_Flail] = 1;

                std::cout << "\n Gaia is bored...";
            }
        }

        /*
        for (int cou_E = 0; cou_E < IO.Efferent_Count; cou_E++)
        {
            std::cout << "\n[" << cou_E << "] " << tmp_Output_Signals[cou_E][0];

            for (int cou_Chrono = 0; cou_Chrono < tmp_Chrono_Depth; cou_Chrono++)
            {
                std::cout << "\t{" << cou_Chrono << " " << tmp_Validate_Signals_Sum[cou_Chrono][cou_E] << "}";
            }
            std::cout << "]";

        }*/

        /*
        //for (int cou_Raw = (IO.Afferent_Count * 3); cou_Raw < tmp_Raw_Depth; cou_Raw++)
        for (int cou_Raw = 0; cou_Raw < tmp_Raw_Depth; cou_Raw++)
        {
            std::cout << "\n ___  ___ Raw[" << cou_Raw << "]:";
            for (int cou_O = 0; cou_O < tmp_Bulk[0][cou_Raw].size(); cou_O++)
            {
                std::cout << "\n ___  ___  ___ Output[" << cou_O << "]";

                tmp_Count = 0;

                std::cout << " start_Anchor[";
                //if (tmp_Bulk[0][cou_Raw][cou_O].flg_Use == 1)
                if (tmp_Validate_Start_Anchor[cou_Raw][cou_O] == 1)
                {
                    std::cout << "+";
                }
                else
                {
                    std::cout << "-";
                }
                std::cout << "]";

                std::cout << "_" << tmp_Validate_Start_Anchor_Sum[cou_O];

                std::cout << " Match[";
                for (int cou_Chrono = 0; cou_Chrono < tmp_Chrono_Depth; cou_Chrono++)
                {
                    //tmp_Count += tmp_Bulk[cou_Chrono][cou_Raw][cou_O].Match.D;


                    if (tmp_Bulk[cou_Chrono][cou_Raw][cou_O].Match.D == 1)
                    {
                        std::cout << "+";
                    }
                    else
                    {
                        std::cout << "-";
                    }

                }
                std::cout << "]";
                //std::cout << " Match Count: " << tmp_Count;
            }
        }
        */
        std::cout << "\n\n______\n";
        //std::system("PAUSE");
    }

    int get_Output_Signals(int p_Index)
    {
        return Output_Signals[p_Index];
    }

    //Encodes an input set.
    void eval(std::string p_FName)
    {
        //Generate the current trajectory prediction.
        TSG.eval(0);

        std::vector<std::vector<u_Data>> tmp_Prediction;

        tmp_Prediction = get_Current_Projection(0);

        //p_ProtoGaia->output_AE();

        //output_Deviation_Mapping();

        std::vector<float> tmp_Deviation_Set;

        tmp_Deviation_Set = get_Current_Deviation_Set();

        //copy_Deviation(1);
        copy_Deviation(1);

        //TSG.output_IO(1);

        //Get the deviation search.
        TSG.eval(1);

        evaluate_Traces(p_FName);

        /*
        for (int cou_D = 0; cou_D < tmp_Deviation_Set.size(); cou_D++)
        {
            //copy_Deviation(1);
            std::cout << "\n\nDeviation_Set[" << cou_D << "]";
            copy_Deviation_Index(1, cou_D);

            TSG.output_IO(1);

            //Get the deviation search.
            TSG.eval(1);

            evaluate_Traces();

        }
        */
        //TSG.output_Bulk(1);

    }

    int register_Afferent()
    {
        int tmp_Return = IO.register_Afferent();
        return tmp_Return;
    }

    int register_Efferent()
    {
        return IO.register_Efferent();
    }

    void add_Afferent_Granulation(double p_Bottom, double p_Top, int p_Index = -1)
    {
        return IO.add_Afferent_Granulation(p_Bottom, p_Top, p_Index);
    }

    void set_Depth(int p_Depth)
    {
        IO.set_Depth(p_Depth);
    }

    void set_Chrono_Depth(int p_Chrono_Depth)
    {
        TSG.set_Chrono_Depth(p_Chrono_Depth);
    }

    void update_MSC_Depth() 
    { 
        TSG.update_MSC_Depth(); 
    }

    int get_Chrono_Depth()
    {
        return TSG.Chrono_Depth;
    }

    std::vector<std::vector<u_Data>> get_Current_Projection(int p_RF)
    {
        return TSG.RF[p_RF].Output;
    }

    void output_Current_Projection(int p_RF)
    {
        std::vector<std::vector<u_Data>> tmp_Projection;

        tmp_Projection = get_Current_Projection(p_RF);

        std::cout << "\n Current_Projection[" << p_RF << "]:";
        for (int cou_C = 0; cou_C < tmp_Projection.size(); cou_C++)
        {
            std::cout << "\n";

            for (int cou_I = 0; cou_I < tmp_Projection[cou_C].size(); cou_I++)
            {
                std::cout << " " << tmp_Projection[cou_C][cou_I].D;
            }
        }
    }

    std::vector<u_Data> get_Random_Raw_Projection(int p_Raw)
    {
        return TSG.get_Random_Raw_Projection(p_Raw);
    }

    void shift_Data()
    {
        IO.shift_Data();
        TSG.shift_Data();
    }
    
    void set_Afferent_Value(int p_Index, float p_Value, bool p_Con = 1, bool p_Gran = 1, bool p_Delt = 1)
    {
        IO.set_Afferent_Value(p_Index, p_Value);

        //The TSG does straight indexing, so we step by three for con/gran/delta
        int tmp_Index = (p_Index * 3);

        //---if (p_Con) { TSG.set_Input_Index((tmp_Index + 0), IO.Afferent[p_Index]->get_Value_Data_uint64_t()); }
        /*-*/if (p_Con) { TSG.set_Input_Index(0, (tmp_Index + 0), IO.Afferent[p_Index]->get_Value_Granulated_uint64_t()); }
        if (p_Gran) { TSG.set_Input_Index(0, (tmp_Index + 1), IO.Afferent[p_Index]->get_Value_Granulated_uint64_t()); }
        if (p_Delt) { TSG.set_Input_Index(0, (tmp_Index + 2), IO.Afferent[p_Index]->get_Value_Delta_uint64_t()); }


        //---std::cout << "\n set_Afferent_Value(" << p_Index << ", " << p_Value << "): tmp_Index: " << tmp_Index;
    }
    
    //The index is passed as the index of the Efferent array, which is separated from the Afferent. Meaning the steps are not in sync.
    //The tmp_Index holds the offset needed to point to the correct index in the straight-indexing of the TSG.
    void set_Efferent_Value(int p_Index, float p_Value)
    {
        IO.set_Efferent_Value(p_Index, p_Value);

        //The TSG does straight indexing, so we step by three for con/gran/delta
        int tmp_Index = ((IO.Afferent_Count) * 3) + (p_Index * 3);

        //---std::cout << "\n set_Efferent_Value(" << p_Index << ", " << p_Value << "): tmp_Index: " << tmp_Index;

        TSG.set_Input_Index(0, (tmp_Index + 0), IO.Efferent[p_Index]->get_Value_Data_uint64_t());
        TSG.set_Input_Index(0, (tmp_Index + 1), IO.Efferent[p_Index]->get_Value_Granulated_uint64_t());
        TSG.set_Input_Index(0, (tmp_Index + 2), IO.Efferent[p_Index]->get_Value_Delta_uint64_t());
    }

    void output_Deviation_Mapping()
    {
        IO.output_Deviation_Mapping();
    }

    std::vector<float> get_Current_Deviation_Set()
    {
        return IO.get_Current_Deviation_Set();
    }

    void copy_Deviation(int p_RF)
    {
        std::vector<float> tmp_Current_Deviation_Set = get_Current_Deviation_Set();

        //---std::cout << "\n copy_Deviation(" << p_RF << ")";

        //The delta are stored in [2]

        for (int cou_Chron = 0; cou_Chron < TSG.Chrono_Depth; cou_Chron++)
        {
            TSG.shift_Data(p_RF, -9999);

            //TSG.set_Input_Index(p_RF, 0, IO.Afferent[0]->get_Value_Data());
            //TSG.set_Input_Index(p_RF, 1, IO.Afferent[0]->get_Value_Granulated());

            for (int cou_A = 1; cou_A < (tmp_Current_Deviation_Set.size() - 1); cou_A++)
            {
                //---std::cout << "\n -~- cou_A " << cou_A;
                //---std::cout << "\n -~- tmp_Current_Deviation_Set[cou_A] " << tmp_Current_Deviation_Set[cou_A];
                int tmp_Index = (cou_A * 3);

                //---std::cout << " -~- tmp_Index " << tmp_Index;

                TSG.set_Input_Index(p_RF, (tmp_Index + 2), tmp_Current_Deviation_Set[cou_A]);
            }
        }
        //---TSG.output_IO(p_RF);
    }

    void copy_Deviation_Index(int p_RF, int p_Index)
    {
        float tmp_Current_Deviation = IO.get_Current_Afferent_Deviation(p_Index);

        //---std::cout << "\n copy_Deviation(" << p_RF << ")";

        //The delta are stored in [2]

        TSG.shift_Data(p_RF, -9999);

        for (int cou_Chron = 1; cou_Chron < (TSG.Chrono_Depth - 1); cou_Chron++)
        {
            TSG.shift_Data(p_RF, -9999);

            //TSG.set_Input_Index(p_RF, 0, IO.Afferent[0]->get_Value_Data());
            //TSG.set_Input_Index(p_RF, 1, IO.Afferent[0]->get_Value_Granulated());

            //---std::cout << "\n -~- cou_A " << cou_A;
            //---std::cout << "\n -~- tmp_Current_Deviation_Set[cou_A] " << tmp_Current_Deviation_Set[cou_A];
            int tmp_Index = (p_Index * 3);

            //---std::cout << " -~- tmp_Index " << tmp_Index;

            TSG.set_Input_Index(p_RF, (tmp_Index + 2), tmp_Current_Deviation);
        }

        TSG.shift_Data(p_RF, -9999);
        //---TSG.output_IO(p_RF);
    }

    void output_TSG()
    {
        TSG.output_Everything();
    }

    void output_NNet()
    {
        TSG.NT4_Core.output_Node_Network();
    }

    void output_AE()
    {
        IO.output_AE();
    }
};





















void setup_Map(c_Map_Sim * p_Map)
{
    p_Map->add_Sensor((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()));
    p_Map->add_Sensor((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()));
    p_Map->add_Sensor((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()));
    p_Map->add_Sensor((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()));
    p_Map->add_Sensor((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()));
    p_Map->add_Sensor((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()));

    //p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), 75);// (rand() % 250));
    p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), (rand() % 70));
    p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), (rand() % 500));
    p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), (rand() % 70));
    p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), (rand() % 500));
    p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), (rand() % 70));
    p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), (rand() % 500));

    //p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), 10);
    //p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), 500);
    //p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), 10);
    //p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), 500);
    //p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), 10);
    //p_Map->add_Actuator((rand() % p_Map->get_Width()), (rand() % p_Map->get_Height()), 500);

    p_Map->turn_Actuator_On(0);
    p_Map->turn_Actuator_On(1);
    p_Map->turn_Actuator_On(2);
    p_Map->turn_Actuator_On(3);
    p_Map->turn_Actuator_On(4);
    p_Map->turn_Actuator_On(5);
}

void setup_Gaia(c_Homeostasis_Module * p_ProtoGaia, int p_Chrono_Depth, int p_IO_Depth)
{
    for (int cou_IO = 0; cou_IO < p_IO_Depth; cou_IO++)
    {
        /*-*/std::cout << "\n <A[" << cou_IO << "]> " << p_ProtoGaia->register_Afferent();

        p_ProtoGaia->add_Afferent_Granulation(79, 80, cou_IO);
        p_ProtoGaia->add_Afferent_Granulation(75, 90, cou_IO);
        p_ProtoGaia->add_Afferent_Granulation(50, 100, cou_IO);
        p_ProtoGaia->add_Afferent_Granulation(25, 110, cou_IO);
        p_ProtoGaia->add_Afferent_Granulation(0, 120, cou_IO);
        p_ProtoGaia->add_Afferent_Granulation(-500, 500, cou_IO);
    }
    for (int cou_IO = 0; cou_IO < p_IO_Depth; cou_IO++)
    {
        /*-*/std::cout << "\n <E[" << cou_IO << "]> " << p_ProtoGaia->register_Efferent();
    }

    p_ProtoGaia->init(p_Chrono_Depth);
}



















std::vector<u_Data> generate_Random_Prediction(int p_Chrono_Depth, float p_Low = 0.0, float p_High = 250.0)
{
    int tmp_Diff = p_High = p_Low;
    std::vector<u_Data> tmp_Random_Projection;

    tmp_Random_Projection.resize(p_Chrono_Depth);

    float tmp_Ran = 0;

    tmp_Ran = (rand() % tmp_Diff) + p_Low;
    tmp_Random_Projection[0].D = tmp_Ran;

    for (int cou_Chrono = 1; cou_Chrono < p_Chrono_Depth; cou_Chrono++)
    {
        tmp_Ran = (rand() % 25) * (-1 * ((rand() % 2) - 1));
        tmp_Random_Projection[cou_Chrono].D = tmp_Random_Projection[cou_Chrono - 1].D + tmp_Ran;
        if (tmp_Random_Projection[cou_Chrono].D < p_Low)
        {
            tmp_Ran = (rand() % 25) + 5;
            tmp_Random_Projection[cou_Chrono].D += tmp_Ran;
        }
        if (tmp_Random_Projection[cou_Chrono].D > p_High)
        {
            tmp_Ran = (rand() % 25) + 5;
            tmp_Random_Projection[cou_Chrono].D -= tmp_Ran;
        }
    }
    return tmp_Random_Projection;
}






float calc_Mean_Squared_Error(float p_Prediction, float p_Actual)
{
    return ((p_Actual - p_Prediction) * (p_Actual - p_Prediction));
}


void gather_ProtoGaia_Input(c_Homeostasis_Module* p_ProtoGaia, c_Map_Sim* p_Map)
{
    p_ProtoGaia->set_Afferent_Value(0, p_Map->get_Sensor_Data(0));
    p_ProtoGaia->set_Afferent_Value(1, p_Map->get_Sensor_Data(1));
    p_ProtoGaia->set_Afferent_Value(2, p_Map->get_Sensor_Data(2));
    p_ProtoGaia->set_Afferent_Value(3, p_Map->get_Sensor_Data(3));
    p_ProtoGaia->set_Afferent_Value(4, p_Map->get_Sensor_Data(4));
    p_ProtoGaia->set_Afferent_Value(5, p_Map->get_Sensor_Data(5));

    p_ProtoGaia->set_Efferent_Value(0, p_Map->get_Actuator_State(0));
    p_ProtoGaia->set_Efferent_Value(1, p_Map->get_Actuator_State(1));
    p_ProtoGaia->set_Efferent_Value(2, p_Map->get_Actuator_State(2));
    p_ProtoGaia->set_Efferent_Value(3, p_Map->get_Actuator_State(3));
    p_ProtoGaia->set_Efferent_Value(4, p_Map->get_Actuator_State(4));
    p_ProtoGaia->set_Efferent_Value(5, p_Map->get_Actuator_State(5));
}



void fill_Chrono(c_Homeostasis_Module* p_ProtoGaia, c_Map_Sim* p_Map, int p_Step_Count)
{
    for (int cou_Actuator = 0; cou_Actuator < 6; cou_Actuator++)
    {
        p_Map->turn_Actuator_Off(cou_Actuator);
    }

    p_Map->set_Map_Temp(100);

    for (int cou_Index = 0; cou_Index < (p_ProtoGaia->get_Chrono_Depth() * 2); cou_Index++)
    {
        for (int cou_N = 0; cou_N < p_Step_Count; cou_N++)
        {
            p_Map->update();
        }
        //---p_Map->view_Map();

        p_ProtoGaia->shift_Data();

        gather_ProtoGaia_Input(p_ProtoGaia, p_Map);
        //---p_ProtoGaia->output_AE();
    }
}

void train_Actuators(c_Homeostasis_Module* p_ProtoGaia, c_Map_Sim* p_Map, int p_Step_Count, int p_Train_Depth)
{
    fill_Chrono(p_ProtoGaia, p_Map, p_Step_Count);

    for (int cou_Actuator = 0; cou_Actuator < 6; cou_Actuator++)
    {

        p_Map->set_Map_Temp(0.0);
        p_Map->turn_Actuator_On(cou_Actuator);
        std::cout << "\n\n";
        for (int cou_Index = 0; cou_Index < p_Train_Depth; cou_Index++)
        {
            for (int cou_N = 0; cou_N < p_Step_Count; cou_N++)
            {
                p_Map->update();
            }
            //---p_Map->view_Map();

            for (int cou_A = 0; cou_A < 100; cou_A++)
            {
                std::cout << char(8);
            }

            std::cout << " Training Actuator[" << cou_Actuator << "] / 6 ";
            std::cout << " 0 Degrees ";
            std::cout << " ......Training Step [" << cou_Index << " / " << p_Train_Depth << "]...";

            p_ProtoGaia->shift_Data();

            gather_ProtoGaia_Input(p_ProtoGaia, p_Map);

            p_ProtoGaia->encode();
            //---p_ProtoGaia->output_AE();
        }


        p_Map->set_Map_Temp(250);
        p_Map->turn_Actuator_On(cou_Actuator);

        for (int cou_Index = 0; cou_Index < p_Train_Depth; cou_Index++)
        {
            for (int cou_N = 0; cou_N < p_Step_Count; cou_N++)
            {
                p_Map->update();
            }
            //---p_Map->view_Map();

            for (int cou_A = 0; cou_A < 100; cou_A++)
            {
                std::cout << char(8);
            }
            std::cout << " Training Actuator[" << cou_Actuator << "] / 6 ";
            std::cout << " 250 Degrees ";
            std::cout << " ......Training Step [" << cou_Index << " / " << p_Train_Depth << "]...";


            p_ProtoGaia->shift_Data();

            gather_ProtoGaia_Input(p_ProtoGaia, p_Map);

            p_ProtoGaia->encode();
            //---p_ProtoGaia->output_AE();
        }

        p_Map->turn_Actuator_Off(cou_Actuator);
    }
    int tmp_Actuator = 0;

}



void random_Training(c_Homeostasis_Module* p_ProtoGaia, c_Map_Sim* p_Map, int p_Step_Count, int p_Train_Count)
{
    fill_Chrono(p_ProtoGaia, p_Map, p_Step_Count);

    std::cout << "\n\n Random Training:\n";
    for (int cou_Index = 0; cou_Index < p_Train_Count; cou_Index++)
    {
        for (int cou_N = 0; cou_N < p_Step_Count; cou_N++)
        {
            p_Map->update();
        }
        //---Map.view_Map();

        for (int cou_A = 0; cou_A < 100; cou_A++)
        {
            std::cout << char(8);
        }
        std::cout << "[" << cou_Index << " / " << p_Train_Count << "]";

        p_ProtoGaia->shift_Data();

        gather_ProtoGaia_Input(p_ProtoGaia, p_Map);


        if (!(p_Train_Count % 10))
        {
            for (int cou_R = 0; cou_R < 6; cou_R++)
            {
                p_Map->turn_Actuator_Off(cou_R);
            }
            p_Map->turn_Actuator_On(rand() % 6);
            p_Map->turn_Actuator_On(rand() % 6);
        }


        p_ProtoGaia->encode();
        //---ProtoGaia.output_AE();
    }
}

void null_Hypo(c_Homeostasis_Module* p_ProtoGaia, c_Map_Sim* p_Map, int p_Step_Count, int p_Train_Count, int p_Seed, std::string p_FName = "")
{
    //Ensures that for the hypo the start is always the same.
    p_Map->set_Map_Temp(100);

    c_Historical_DB Projection_DB;
    c_Historical_DB Hist_DB;

    Projection_DB.set_Chrono_Depth(p_ProtoGaia->get_Chrono_Depth());
    Projection_DB.set_Raw_Depth(p_ProtoGaia->TSG.Raw_Depth);

    Hist_DB.set_Chrono_Depth(p_ProtoGaia->get_Chrono_Depth());
    Hist_DB.set_Raw_Depth(p_ProtoGaia->TSG.Raw_Depth);

    std::string tmp_FName = "./GaiaTesting/null_hypo." + p_FName + ".ssv";
    std::ofstream nulll_File(tmp_FName, std::ios::trunc);
    nulll_File.close();

    float tmp_D_MSE_Total = 0.0;
    float tmp_D_MSE_Total_Count = 0.0;

    std::cout << "\n\n Generating NULL Hypothosis Set:\n";

    srand(p_Seed);
    for (int cou_Index = 0; cou_Index < p_Train_Count; cou_Index++)
    {
        for (int cou_N = 0; cou_N < p_Step_Count; cou_N++)
        {
            p_Map->update();
        }
        //---p_Map->view_Map();

        p_ProtoGaia->shift_Data();

        gather_ProtoGaia_Input(p_ProtoGaia, p_Map);



        for (int cou_R = 0; cou_R < 6; cou_R++)
        {
            if (!(rand() % 25))
            {
                if (!(rand() % 2))
                {
                    p_Map->turn_Actuator_On(cou_R);
                }
                else
                {
                    p_Map->turn_Actuator_Off(cou_R);
                }
                cou_R = 6;
            }
        }


        //Projection_DB.add_To_Hist(p_ProtoGaia->get_Current_Projection());
        //---Projection_DB.add_To_Hist(tmp_Random_Projection);

        //---std::cout << "\n\n Projection History";
        //---Projection_DB.output_DB();

        Hist_DB.add_To_Hist(p_ProtoGaia->TSG.RF[0].Input);
        //---std::cout << "\n\n Ground Truth History";
        //---Hist_DB.output_DB();

        //So we have to take the prediction from Chrono_Depth steps ago. This will be compared to the entry for the current.

        std::vector<u_Data> tmp_Prediction;
        std::vector<u_Data> tmp_Truth;

        float tmp_Current_MSE = 0.0;
        float tmp_Current_MSE_Count = 0.0;

        std::vector<float> tmp_Current_MSE_Raw;
        std::vector<float> tmp_Current_MSE_Raw_Count;


        if (cou_Index >= Hist_DB.get_Chrono_Depth())
        {

            std::string tmp_FName = "./GaiaTesting/null_hypo." + p_FName + ".ssv";
            std::ofstream null_File(tmp_FName, std::ios::app);

            tmp_Current_MSE = 0.0;
            tmp_Current_MSE_Count = 0.0;

            //---std::cout << "\n Hist: ";
            for (int cou_Raw = 0; cou_Raw < p_ProtoGaia->TSG.Raw_Depth; cou_Raw++)
            {
                tmp_Current_MSE_Raw.clear();
                tmp_Current_MSE_Raw_Count.clear();

                tmp_Current_MSE_Raw.resize(p_ProtoGaia->TSG.Raw_Depth);
                tmp_Current_MSE_Raw_Count.resize(p_ProtoGaia->TSG.Raw_Depth);

                for (int cou_Stat = 0; cou_Stat < 25; cou_Stat++)
                {
                    //tmp_Prediction = generate_Random_Prediction(Hist_DB.get_Chrono_Depth());
                    tmp_Prediction = p_ProtoGaia->get_Random_Raw_Projection(cou_Raw);
                    tmp_Truth = Hist_DB.get_Entry(cou_Index, cou_Raw);
                    //---std::cout << "\n -=- [" << cou_Raw << "]: ";
                    for (int cou_A = 0; cou_A < tmp_Prediction.size(); cou_A++)
                    {
                        //---std::cout << " <" << tmp_Prediction[cou_A].D << " _ ";
                        //---std::cout << tmp_Truth[cou_A].D << " _ ";
                        //---std::cout << calc_Mean_Squared_Error(tmp_Prediction[cou_A].D, tmp_Truth[cou_A].D) << "> ";
                        tmp_D_MSE_Total += calc_Mean_Squared_Error(tmp_Prediction[cou_A].D, tmp_Truth[cou_A].D);
                        tmp_Current_MSE += calc_Mean_Squared_Error(tmp_Prediction[cou_A].D, tmp_Truth[cou_A].D);
                        tmp_Current_MSE_Raw[cou_A] += calc_Mean_Squared_Error(tmp_Prediction[cou_A].D, tmp_Truth[cou_A].D);
                        tmp_D_MSE_Total_Count++;
                        tmp_Current_MSE_Count++;
                        tmp_Current_MSE_Raw_Count[cou_A]++;

                    }
                }
                /*
                for (int cou_A = 0; cou_A < tmp_Prediction.size(); cou_A++)
                {
                    null_File << (tmp_Current_MSE_Raw[cou_A] / tmp_Current_MSE_Raw_Count[cou_A]);

                    if (cou_A < (tmp_Prediction.size() - 1))
                    {
                        null_File << " ";
                    }
                }
                */
            }

            null_File << (tmp_Current_MSE / tmp_Current_MSE_Count);
            null_File << "\n";

            null_File.close();
        }

        for (int cou_A = 0; cou_A < 100; cou_A++)
        {
            std::cout << char(8);
        }

        std::cout << "[" << cou_Index << " / " << p_Train_Count << "] MSE: " << (tmp_D_MSE_Total / tmp_D_MSE_Total_Count) << "     ";


        //---p_ProtoGaia->output_AE();
    }
}

void pred_Hypo(c_Homeostasis_Module* p_ProtoGaia, c_Map_Sim* p_Map, int p_Step_Count, int p_Train_Count, int p_Seed, std::string p_FName = "")
{
    fill_Chrono(p_ProtoGaia, p_Map, p_Step_Count);

    c_Historical_DB Projection_DB;
    c_Historical_DB Hist_DB;

    Projection_DB.set_Chrono_Depth(p_ProtoGaia->get_Chrono_Depth());
    Projection_DB.set_Raw_Depth(p_ProtoGaia->TSG.Raw_Depth);
    Hist_DB.set_Chrono_Depth(p_ProtoGaia->get_Chrono_Depth());
    Hist_DB.set_Raw_Depth(p_ProtoGaia->TSG.Raw_Depth);

    float tmp_MSE_Total = 0.0;
    float tmp_MSE_Total_Count = 0.0;

    std::string tmp_FName = "./GaiaTesting/prediction_hypo." + p_FName + ".ssv";
    std::ofstream predd_File(tmp_FName, std::ios::trunc);
    predd_File.close();

    std::cout << "\n\n Gathering Predictive Hypothosis Set:\n";
    srand(p_Seed);
    for (int cou_Index = 0; cou_Index < p_Train_Count; cou_Index++)
    {
        for (int cou_N = 0; cou_N < p_Step_Count; cou_N++)
        {
            p_Map->update();
        }
        //---p_Map->view_Map();

        p_ProtoGaia->shift_Data();

        gather_ProtoGaia_Input(p_ProtoGaia, p_Map);

        for (int cou_R = 0; cou_R < 6; cou_R++)
        {
            if (!(rand() % 25))
            {
                if (!(rand() % 2))
                {
                    p_Map->turn_Actuator_On(cou_R);
                }
                else
                {
                    p_Map->turn_Actuator_Off(cou_R);
                }
                cou_R = 6;
            }
        }

        p_ProtoGaia->eval(p_FName);

        Projection_DB.add_To_Hist(p_ProtoGaia->get_Current_Projection(0));

        //---std::cout << "\n\n Projection History";
        //---Projection_DB.output_DB();

        Hist_DB.add_To_Hist(p_ProtoGaia->TSG.RF[0].Input);
        //---std::cout << "\n\n Ground Truth History";
        //---Hist_DB.output_DB();

        //So we have to take the prediction from Chrono_Depth steps ago. This will be compared to the entry for the current.

        float tmp_Current_MSE = 0.0;
        float tmp_Current_MSE_Count = 0.0;

        std::vector<u_Data> tmp_Prediction;
        std::vector<u_Data> tmp_Truth;

        std::vector<float> tmp_Current_MSE_Raw;
        std::vector<float> tmp_Current_MSE_Raw_Count;

        tmp_Current_MSE_Raw.clear();
        tmp_Current_MSE_Raw_Count.clear();


        if (cou_Index >= Hist_DB.get_Chrono_Depth())
        {
            std::string tmp_FName = "./GaiaTesting/prediction_hypo." + p_FName + ".ssv";
            std::ofstream pred_File(tmp_FName, std::ios::app);

            tmp_Current_MSE = 0.0;
            tmp_Current_MSE_Count = 0.0;

            //---std::cout << "\n Hist: ";
            for (int cou_Raw = 0; cou_Raw < p_ProtoGaia->TSG.Raw_Depth; cou_Raw++)
            {
                tmp_Current_MSE_Raw.resize(p_ProtoGaia->TSG.Raw_Depth);
                tmp_Current_MSE_Raw_Count.resize(p_ProtoGaia->TSG.Raw_Depth);

                tmp_Current_MSE_Raw.clear();
                tmp_Current_MSE_Raw_Count.clear();

                tmp_Prediction = Projection_DB.get_Entry((cou_Index - Hist_DB.get_Chrono_Depth()), cou_Raw);
                tmp_Truth = Hist_DB.get_Entry(cou_Index, cou_Raw);
                //---std::cout << "\n -=- [" << cou_Raw << "]: ";
                for (int cou_A = 0; cou_A < tmp_Prediction.size(); cou_A++)
                {
                    //---std::cout << " <" << tmp_Prediction[cou_A].D << " _ ";
                    //---std::cout << tmp_Truth[cou_A].D << " _ ";
                    //---std::cout << calc_Mean_Squared_Error(tmp_Prediction[cou_A].D, tmp_Truth[cou_A].D) << "> ";
                    tmp_Current_MSE += calc_Mean_Squared_Error(tmp_Prediction[cou_A].D, tmp_Truth[cou_A].D);
                    tmp_MSE_Total += calc_Mean_Squared_Error(tmp_Prediction[cou_A].D, tmp_Truth[cou_A].D);
                    //tmp_Current_MSE_Raw += calc_Mean_Squared_Error(tmp_Prediction[cou_A].D, tmp_Truth[cou_A].D);
                    tmp_MSE_Total_Count++;
                    tmp_Current_MSE_Count++;
                    //tmp_Current_MSE_Raw_Count++;

                    /*
                    pred_File << calc_Mean_Squared_Error(tmp_Prediction[cou_A].D, tmp_Truth[cou_A].D);

                    if (cou_A < (tmp_Prediction.size() - 1))
                    {
                        pred_File << " ";
                    }
                    */
                }
            }

            //pred_File << "\n";
            pred_File << (tmp_Current_MSE / tmp_Current_MSE_Count) << "\n";
            pred_File.close();
        }

        for (int cou_A = 0; cou_A < 100; cou_A++)
        {
            std::cout << char(8);
        }
        //std::cout << "[" << cou_Index << " / " << p_Train_Count << " MSE: " << tmp_MSE_Total << "   " << tmp_MSE_Total_Count << "    ";
        std::cout << "[" << cou_Index << " / " << p_Train_Count << " MSE: " << (tmp_MSE_Total / tmp_MSE_Total_Count) << "           ";
    }

}

void generate_NULL_Hypo_Gaia(c_Map_Sim* p_Map, int p_Step_Count, int p_Random_Seed, int p_Test_Depth, std::string p_FName)
{
    std::cout << "\n\n NULL GEN:\n";


    std::cout << " Step_Count: " << p_Step_Count << " Seed: " << p_Random_Seed;
    srand(p_Random_Seed);

    p_Map->set_Map_Temp(25);

    for (int cou_Index = 0; cou_Index < p_Test_Depth; cou_Index++)
    {
        //p_Map->set_Temp(tmp_Ran_X, tmp_Ran_Y, tmp_Ran_Temp);

        for (int cou_A = 0; cou_A < p_Map->get_Actuator_Count(); cou_A++)
        {
            if (rand() % 2)
            {
                p_Map->turn_Actuator_On(cou_A);
            }
            else
            {
                p_Map->turn_Actuator_Off(cou_A);
            }
        }

        for (int cou_N = 0; cou_N < p_Step_Count; cou_N++)
        {
            p_Map->update();
        }
        /*-*/p_Map->view_Map();

        std::string tmp_FName_Root = "./GaiaTesting/";

        for (int cou_E = 0; cou_E < p_Map->get_Actuator_Count(); cou_E++)
        {
            std::ofstream SSF;
            std::string tmp_SSFName = tmp_FName_Root + "signals/" + p_FName + ".NULL.sig." + std::to_string(cou_E) + ".ssv";
            SSF.open(tmp_SSFName, std::ios::app);

            if (p_Map->get_Actuator_State(cou_E) == 1)
            {
                SSF << (cou_E + 1.25) << "\n";
            }
            else
            {
                SSF << "0\n";
            }

            SSF.close();
        }

        std::string tmp_Map_Temps_Name = tmp_FName_Root + "temps/NULL.temps";
        p_Map->write_Map_Temps(tmp_Map_Temps_Name);

        std::string tmp_Map_MSE_Name = tmp_FName_Root + "mse/NULL.MSE";
        p_Map->write_Map_MSE(tmp_Map_MSE_Name);

        std::string tmp_Map_Map_Name = tmp_FName_Root + "/NULL";
        p_Map->write_Map_Tiles(tmp_Map_Map_Name);
    }
}

void manual_Mode(c_Homeostasis_Module* p_ProtoGaia, c_Map_Sim* p_Map, int p_Step_Count, int p_Random_Seed, int p_Test_Depth, std::string p_FName, bool p_Output_Bulk)
{


    std::cout << "\n\n Manual OVERRIDE ENGAGED HYAWW:\n";
    
    std::cout << "\n\n Nvm, random running:\n";

    std::cout << " Step_Count: " << p_Step_Count << " Seed: " << p_Random_Seed;
    srand(p_Random_Seed);

    p_Map->set_Map_Temp(25);

    for (int cou_E = 0; cou_E < p_ProtoGaia->IO.Efferent_Count; cou_E++)
    {
        p_ProtoGaia->set_Efferent_Value(cou_E, -1);
    }

    for (int cou_Index = 0; cou_Index < p_Test_Depth; cou_Index++)
    {
        //p_Map->set_Temp(tmp_Ran_X, tmp_Ran_Y, tmp_Ran_Temp);


        for (int cou_N = 0; cou_N < p_Step_Count; cou_N++)
        {
            p_Map->update();
        }
        /*-*/p_Map->view_Map();

        //std::cout << "\n Rand " << tmp_Ran_Streak << " / " << tmp_Ran_Streak_Depth << " Temp: " << tmp_Ran_Temp << " @ (" << tmp_Ran_X << ", " << tmp_Ran_Y << ")";

        p_ProtoGaia->shift_Data();

        gather_ProtoGaia_Input(p_ProtoGaia, p_Map);

        /*
        p_ProtoGaia->set_Afferent_Value(0, (rand() % 200));
        p_ProtoGaia->set_Afferent_Value(1, (rand() % 200));
        p_ProtoGaia->set_Afferent_Value(2, (rand() % 200));
        p_ProtoGaia->set_Afferent_Value(3, (rand() % 200));
        p_ProtoGaia->set_Afferent_Value(4, (rand() % 200));
        p_ProtoGaia->set_Afferent_Value(5, (rand() % 200));
        */
        
        p_ProtoGaia->eval(p_FName);

        if (p_Output_Bulk == 1)
        {
            p_ProtoGaia->write_Bulk(p_FName);
        }

        int tmp_ONOFF = 0;
        for (int cou_R = 0; cou_R < 6; cou_R++)
        {
            tmp_ONOFF = 0;
            //std::cout << "\n Enter A[" << cou_R << "] 1/0:";
            //std::cin >> tmp_ONOFF;

            tmp_ONOFF = p_ProtoGaia->get_Output_Signals(cou_R);
            //std::cout << "\n A[" << cou_R << "]: " << tmp_ONOFF;

            

            if (tmp_ONOFF > 0)
            {
                p_Map->turn_Actuator_On(cou_R);
            }
            else
            {
                p_Map->turn_Actuator_Off(cou_R);
            }
            /*
            if (!(rand() % 10))
            {
                if (!(rand() % 2))
                {
                    p_Map->turn_Actuator_On(cou_R);
                }
                else
                {
                    p_Map->turn_Actuator_Off(cou_R);
                }
                cou_R = 6;
            }
            */
        }

        std::string tmp_FName_Root = "./GaiaTesting/";

        for (int cou_E = 0; cou_E < p_ProtoGaia->IO.Efferent_Count; cou_E++)
        {
            std::ofstream SSF;
            std::string tmp_SSFName = tmp_FName_Root + "signals/" + p_FName + ".sig." + std::to_string(cou_E) + "." + std::to_string(p_Map->get_Actuator_Temp(cou_E)) + ".ssv";
            SSF.open(tmp_SSFName, std::ios::app);

            if (p_ProtoGaia->get_Output_Signals(cou_E) == 1)
            {
                SSF << p_Map->get_Actuator_Temp(cou_E) << "\n";
            }
            else
            {
                SSF << "0\n";
            }

            SSF.close();
        }

        std::string tmp_Map_Temps_Name = tmp_FName_Root + "temps/" + p_FName + ".temp";
        p_Map->write_Map_Temps(tmp_Map_Temps_Name);

        std::string tmp_Map_MSE_Name = tmp_FName_Root + "mse/" + p_FName + ".MSE";
        p_Map->write_Map_MSE(tmp_Map_MSE_Name);

        std::string tmp_Map_Map_Name = tmp_FName_Root + p_FName + ".map";
        p_Map->write_Map_Tiles(tmp_Map_Map_Name);

        p_ProtoGaia->encode();

        /*
        for (int cou_R = 0; cou_R < tmp_Prediction.size(); cou_R++)
        {
            std::cout << "\n cou_R[" << cou_R << "]";

            for (int cou_I = 0; cou_I < tmp_Prediction[cou_R].size(); cou_I++)
            {
                std::cout << " (" << tmp_Prediction[cou_R][cou_I].D << "_";

                if (tmp_Prediction[cou_R][cou_I].D > 0) { std::cout << "(-1)"; }
                if (tmp_Prediction[cou_R][cou_I].D == 0) { std::cout << "(0)"; }
                if (tmp_Prediction[cou_R][cou_I].D < 0) { std::cout << "(+1)"; }
                std::cout << ")";
            }
        }*/
    }

}

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

int run_Gaia()
{
    std::string EName = "";
    std::cout << "\n Enter Experiment_Name (string): ";
    /*-*/std::cin >> EName;
    //---EName = "testing";

    int Chrono_Depth = 3;
    std::cout << "\n Enter Chrono_Depth (int): ";
    std::cin >> Chrono_Depth;

    int Step_Count = 100;
    std::cout << "\n Enter Step_Count (int): ";
    std::cin >> Step_Count;
     
    int tmp_Train_Depth = 10;
    std::cout << "\n Enter tmp_Actuator_Train_Depth (int): ";
    std::cin >> tmp_Train_Depth;

    int tmp_RanMax = 250;
    std::cout << "\n Enter tmp_Random_Train_Depth (int): ";
    std::cin >> tmp_RanMax;

    //These are for the prediction tesing portion.
    /*
    int tmp_Null_Run_Count = 1;
    std::cout << "\n Enter # null sets to gen (int): ";
    //---std::cin >> tmp_Null_Run_Count;

    int tmp_Pred_Run_Count = 1;
    std::cout << "\n Enter # prediction sets to gen (int): ";
    //---std::cin >> tmp_Pred_Run_Count;

    int tmp_Stat_Count = 1500;
    std::cout << "\n Enter depth of null sets to gen (int): ";
    //---std::cin >> tmp_Stat_Count;

    int tmp_Test_Count = 1500;
    std::cout << "\n Enter depth of prediction sets to gen (int): ";
    //---std::cin >> tmp_Test_Count;

    */


    int random_Seed_Base = 0;
    int random_Seed = 0;
    std::cout << "\n Enter random_Run_Seed (int): ";
    std::cin >> random_Seed_Base;
    random_Seed = random_Seed_Base;

    int random_Training_Seed_Base = 9000;
    std::cout << "\n Enter random_Training_Seed (int): ";
    std::cin >> random_Training_Seed_Base;

    //The hyperparamters.
    float hyper_Param_Prediction_Action_Potential_Threshold_MSC = 9000;
    std::cout << "\n Enter Prediction hyper_Param_Prediction_Action_Potential_Threshold_MSC (float): ";
    std::cin >> hyper_Param_Prediction_Action_Potential_Threshold_MSC;

    float hyper_Param_Prediction_Modifier_Charge_MSC = 9000;
    std::cout << "\n Enter Prediction hyper_Param_Prediction_Modifier_Charge_MSC (float): ";
    std::cin >> hyper_Param_Prediction_Modifier_Charge_MSC;

    float hyper_Param_Prediction_Action_Potential_Threshold_Chrono = 9000;
    std::cout << "\n Enter Prediction hyper_Param_Prediction_Action_Potential_Threshold_Chrono (float): ";
    std::cin >> hyper_Param_Prediction_Action_Potential_Threshold_Chrono;

    float hyper_Param_Prediction_Modifier_Charge_Chrono = 9000;
    std::cout << "\n Enter Prediction hyper_Param_Prediction_Modifier_Charge_Chrono (float): ";
    std::cin >> hyper_Param_Prediction_Modifier_Charge_Chrono;



    float hyper_Param_Deviation_Action_Potential_Threshold_MSC = 9000;
    std::cout << "\n Enter Deviation hyper_Param_Deviation_Action_Potential_Threshold_MSC (float): ";
    std::cin >> hyper_Param_Deviation_Action_Potential_Threshold_MSC;

    float hyper_Param_Deviation_Modifier_Charge_MSC = 9000;
    std::cout << "\n Enter Deviation hyper_Param_Deviation_Modifier_Charge_MSC (float): ";
    std::cin >> hyper_Param_Deviation_Modifier_Charge_MSC;

    float hyper_Param_Deviation_Action_Potential_Threshold_Chrono = 9000;
    std::cout << "\n Enter Deviation hyper_Param_Deviation_Action_Potential_Threshold_Chrono (float): ";
    std::cin >> hyper_Param_Deviation_Action_Potential_Threshold_Chrono;

    float hyper_Param_Deviation_Modifier_Charge_Chrono = 9000;
    std::cout << "\n Enter Deviation hyper_Param_Deviation_Modifier_Charge_Chrono (float): ";
    std::cin >> hyper_Param_Deviation_Modifier_Charge_Chrono;


    bool output_Bulk = false;
    std::cout << "\n Enter output_Bulk [0 | 1]: ";
    std::cin >> output_Bulk;


    std::string tmp_EName_Full = "./GaiaTesting/" + EName + ".cfg";

    std::ofstream NSF;
    NSF.open(tmp_EName_Full, std::ios::ate);

    NSF << "Chrono_Depth (int): ";
    NSF << Chrono_Depth;

    NSF << "\n Step_Count (int): ";
    NSF << Step_Count;

    NSF << "\n tmp_Actuator_Train_Depth (int): ";
    NSF << tmp_Train_Depth;

    NSF << "\n tmp_Random_Train_Depth (int): ";
    NSF << tmp_RanMax;

    /*
    NSF << "\n # null sets to gen (int): ";
    NSF << tmp_Null_Run_Count;

    NSF << "\n # prediction sets to gen (int): ";
    NSF << tmp_Pred_Run_Count;

    NSF << "\n depth of null sets to gen (int): ";
    NSF << tmp_Stat_Count;

    NSF << "\n depth of prediction sets to gen (int): ";
    NSF << tmp_Test_Count;
    */

    NSF << "\n random_Run_Seed (int): ";
    NSF << random_Seed_Base;
    
    NSF << "\n Enter random_Training_Seed (int): ";
    NSF << random_Training_Seed_Base;
    NSF.close();

    srand(random_Seed);

    //c_Map_Sim Map(25, 25);
    c_Map_Sim Map(25, 25);
    c_Homeostasis_Module ProtoGaia;

    setup_Map(&Map);
    setup_Gaia(&ProtoGaia, Chrono_Depth, 6);

    ProtoGaia.TSG.set_Chrono_APT(0, hyper_Param_Prediction_Action_Potential_Threshold_Chrono);
    ProtoGaia.TSG.set_MSC_APT(0, hyper_Param_Prediction_Action_Potential_Threshold_MSC);
    ProtoGaia.TSG.set_Chrono_MC(0, hyper_Param_Prediction_Modifier_Charge_Chrono);
    ProtoGaia.TSG.set_MSC_MC(0, hyper_Param_Prediction_Modifier_Charge_MSC);

    ProtoGaia.TSG.set_Chrono_APT(1, hyper_Param_Deviation_Action_Potential_Threshold_Chrono);
    ProtoGaia.TSG.set_MSC_APT(1, hyper_Param_Deviation_Action_Potential_Threshold_MSC);
    ProtoGaia.TSG.set_Chrono_MC(1, hyper_Param_Deviation_Modifier_Charge_Chrono);
    ProtoGaia.TSG.set_MSC_MC(1, hyper_Param_Deviation_Modifier_Charge_MSC);


    srand(random_Training_Seed_Base);
    train_Actuators(&ProtoGaia, &Map, Step_Count, tmp_Train_Depth);
    srand(random_Training_Seed_Base);
    random_Training(&ProtoGaia, &Map, Step_Count, tmp_RanMax);

    //std::string tmp_EName = "";
    
    generate_NULL_Hypo_Gaia(&Map, Step_Count, random_Seed, 0, EName);

    while (1)
    {
        manual_Mode(&ProtoGaia, &Map, Step_Count, random_Seed, 1000, EName, output_Bulk);
        return 1;
    }
    
    /*
    random_Seed = random_Seed_Base;
    srand(random_Seed);
    for (int cou_R = 0; cou_R < tmp_Pred_Run_Count; cou_R++)
    {
        EName = EName + "." + std::to_string(random_Seed);

        pred_Hypo(&ProtoGaia, &Map, Step_Count, tmp_Stat_Count, random_Seed, EName);
        random_Seed++;
    }

    random_Seed = random_Seed_Base;
    for (int cou_R = 0; cou_R < tmp_Null_Run_Count; cou_R++)
    {
        EName = EName + "." + std::to_string(random_Seed);

        null_Hypo(&ProtoGaia, &Map, Step_Count, tmp_Stat_Count, random_Seed, EName);
        random_Seed++;
    }*/

    //train_Actuators(&ProtoGaia, &Map, Step_Count, tmp_Train_Depth);
    //random_Training(&ProtoGaia, &Map, Step_Count, tmp_RanMax);





    return 1;
}