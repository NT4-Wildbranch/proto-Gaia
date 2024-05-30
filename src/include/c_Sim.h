

class c_Sim
{
public:

	int Iteration;

	int O2;
	int O2_Delta;

	int Temp;
	int Temp_Delta;

	int O2_Pump;
	int Heater;

	c_Sim()
	{
		Iteration = 0;

		O2 = 5;
		O2_Delta = 1;

		Temp = 5;
		Temp_Delta = 1;

		O2_Pump = 0;
		Heater = 0;
	}

	// Afferent Sensors - Goals
	// O2      | -1/iteration //Breathing   | Goal: 5 | G:5 Y:(4 & 6)      R: (< 4 & > 6)  | Priority: 2  
	// Temp    | -2/iteration //Winter      | Goal: 8 | G:8 Y:(6-7 & 9-10) R: (< 6 & > 10) | Priority: 2  

	// Actuator Systems - Feedback
	// O2 Pump | +4/Iteration [O2]          | -1/Iteration [Temperature] | if (O2 < 5) (Heater = 1) else (Heater = 0)
	// Heater  | +3/Iteration [Temperature] |                            | if (Temp < 8) (Heater = 1) else (Heater = 0)

	int iterate(int p_O2_Pump = -1, int p_Heater = -1)
	{
		Iteration++;

		//Environmental
		int tmp_O2 = O2;
		O2 -= 1; //Breathing

		int tmp_Temp = Temp;
		Temp -= 2; //Winter

		//Calculate actuator effects.
		if (O2_Pump) { O2 += 4; Temp -= 1; } //O2 Pump
		if (Heater) { Temp += 3; } //Heater

		//Calculate the environmental changes based on the previous iterations.
		if (O2 < tmp_O2) { O2_Delta = 0; }
		if (O2 == tmp_O2) { O2_Delta = 1; }
		if (O2 > tmp_O2) { O2_Delta = 2; }

		if (Temp < tmp_Temp) { Temp_Delta = 0; }
		if (Temp == tmp_Temp) { Temp_Delta = 1; }
		if (Temp > tmp_Temp) { Temp_Delta = 2; }

		if ((p_Heater != -1) && (p_O2_Pump != -1))
		{
			Heater = p_Heater;
			O2_Pump = p_O2_Pump;
		}
		else
		{
			//Calculate the actuator state.
			if (tmp_Temp < 8) { Heater = 1; }
			if (tmp_O2 < 5) { O2_Pump = 1; }

			if (tmp_Temp >= 8) { Heater = 0; }
			if (tmp_O2 >= 5) { O2_Pump = 0; }
		}

		return Iteration;
	}

	void output()
	{
		std::cout << "\n";
		std::cout << Iteration;
		std::cout << "\nencode 0 set_input 0 O2 " << O2;
		std::cout << "\nencode 0 set_input 0 O2_Delta " << O2_Delta;

		std::cout << "\nencode 0 set_input 0 Temp " << Temp;
		std::cout << "\nencode 0 set_input 0 Temp_Delta " << Temp_Delta;

		std::cout << "\nencode 0 set_input 0 O2_Pump " << O2_Pump;
		std::cout << "\nencode 0 set_input 0 Heater " << Heater;
	}

	void output_F(std::string p_FName)
	{
		std::ofstream tmp_F;

		tmp_F.open(p_FName, std::ios::app);


		tmp_F << "\n";
		tmp_F << "\noutput_newline";
		tmp_F << "\nset_input 0 ar_O2 " << O2 << " /end/ encode 0 gather_treetop_node 0 ";
		tmp_F << "\nset_input 1 ac_O2_Delta " << O2_Delta << " /end/ encode 1 gather_treetop_node 1 ";
		tmp_F << "\nset_input 2 em_O2_Pump " << O2_Pump << " /end/ encode 2 gather_treetop_node 2 ";

		tmp_F << "\nset_input 3 ar_Temp " << Temp << " /end/ encode 3 gather_treetop_node 3 ";
		tmp_F << "\nset_input 4 ac_Temp_Delta " << Temp_Delta << " /end/ encode 4 gather_treetop_node 4 ";
		tmp_F << "\nset_input 5 em_Heater " << Heater << " /end/ encode 5 gather_treetop_node 5 ";

		tmp_F.close();
	}
};


class c_Cell
{
public:

	float Temp;

	int Type;

	c_Cell()
	{
		Temp = 0;
		Type = 1;
	}
};

class c_Sensor
{
public:

	int X;
	int Y;
	float Previous;
	float Delta;
	float Current;

	c_Sensor()
	{
		X = 0;
		Y = 0;
		Previous = 0;
		Current = 0;
		Delta = 0;
	}

	float get_Delta()
	{
		return Delta;
	}

	void set_Current(float p_Current)
	{
		Previous = Current;
		Current = p_Current;

		Delta = Current - Previous;
	}
};

class c_Actuator
{
public:

	int X;
	int Y;

	bool flg_On_Off;

	float On_Temp;

	float Current;

	c_Actuator()
	{
		X = 0;
		Y = 0;
		Current = 0;
		On_Temp = 100;

		flg_On_Off = false;
	}

	int is_On(){ if (flg_On_Off) { return 1; } else { return -1; } }

	bool turn_Off() { flg_On_Off = false; return flg_On_Off; }
	bool turn_On() { flg_On_Off = true; return flg_On_Off; }

	float get_Temp() { return On_Temp; }
};

class c_Historical_DB
{
	std::vector<std::vector<std::vector<u_Data>>> Hist;
	int Chrono_Depth;
	int Raw_Depth;

public:

	c_Historical_DB()
	{
		Chrono_Depth = 0;
		Raw_Depth = 0;
	}

	void reset()
	{
		Hist.clear();
	}

	void set_Chrono_Depth(int p_Chrono_Depth)
	{
		Chrono_Depth = p_Chrono_Depth;
	}

	void set_Raw_Depth(int p_Raw_Depth)
	{
		Raw_Depth = p_Raw_Depth;
	}

	int get_Chrono_Depth()
	{
		return Chrono_Depth;
	}

	int get_Raw_Depth()
	{
		return Raw_Depth;
	}

	std::vector<u_Data> get_Entry(int p_Step, int p_Raw)
	{
		//Build the entry.
		std::vector<u_Data> return_Data;

		for (int cou_Chrono = 0; cou_Chrono < Chrono_Depth; cou_Chrono++)
		{
			//---std::cout << "\n Chrono[" << cou_Chrono << "]";
			//---std::cout << " " << Hist[cou_Step][cou_Chrono][cou_Raw].D;
			return_Data.push_back(Hist[p_Step][cou_Chrono][p_Raw]);
		}

		return return_Data;
	}

	void add_To_Hist(std::vector<std::vector<u_Data>> p_Data)
	{
		//So we need history for every frame
		//One datatype per history DB
		//We store the projection associated with each index
		//As we step away from the peojections we do the comparison the ground truth to get the scores

		//One frame per tick so far, this is pushed on.
		//One array per frame to hold the prediction or current stats.

		//This tick.
		//Hist.push_back(std::vector<std::vector<u_Data>>());
		Hist.push_back(p_Data);
		return;

		//Hold the current frame chrono block depth.
		Hist[Hist.size() - 1].resize(p_Data.size());

		//Copy the frame data over.
		for (int cou_Chrono = 0; cou_Chrono < p_Data.size(); cou_Chrono++)
		{
			Hist[Hist.size() - 1][cou_Chrono].resize(p_Data[cou_Chrono].size());
			for (int cou_Raw = 0; cou_Raw < p_Data[cou_Chrono].size(); cou_Raw++)
			{
				Hist[Hist.size() - 1][cou_Chrono][cou_Raw] = p_Data[cou_Chrono][cou_Raw];
			}
		}
	}

	int get_Entry_Count()
	{
		return Hist.size();
	}

	void output_DB()
	{
		std::cout << "\n\n output_DB()";
		for (int cou_Step = 0; cou_Step < Hist.size(); cou_Step++)
		{
			std::cout << "\n Index[" << cou_Step << "]";
			for (int cou_Raw = 0; cou_Raw < Raw_Depth; cou_Raw++)
			{
				std::cout << "\n ... Raw[" << cou_Raw << "]";
				for (int cou_Chrono = 0; cou_Chrono < Chrono_Depth; cou_Chrono++)
				{
					//---std::cout << "\n Chrono[" << cou_Chrono << "]";
					std::cout << " " << Hist[cou_Step][cou_Chrono][cou_Raw].D;
				}
			}
		}
	}
};

class c_Map_Sim
{
	std::vector<std::vector<std::vector<c_Cell>>> Map;
	bool Current_Frame;
	bool Next_Frame;

	std::vector<c_Sensor> Sensors;

	std::vector<c_Actuator> Actuators;

	int Width;

	int Height;

	std::string Filename;

public:

	c_Map_Sim(int p_Width, int p_Height)
	{
		Current_Frame = 0;
		Next_Frame = 1;

		Filename = "./Scripts/";

		new_Map(p_Width, p_Height);
	}

	int get_Width() { return Width; }
	int get_Height() { return Height; }

	void swap_Frame()
	{
		Current_Frame = !Current_Frame;
		Next_Frame = !Current_Frame;
		//std::cout << "\n[" << Current_Frame << " " << Next_Frame << "]";
	}

	int add_Sensor(int p_X, int p_Y)
	{
		Sensors.push_back(c_Sensor());

		Sensors[Sensors.size() - 1].X = p_X;
		Sensors[Sensors.size() - 1].Y = p_Y;

		Map[Current_Frame][p_X][p_Y].Type = 2;
		Map[Next_Frame][p_X][p_Y].Type = 2;

		return int(Sensors.size()) - 1;
	}

	int add_Actuator(int p_X, int p_Y, float p_Temp)
	{
		Actuators.push_back(c_Actuator());

		Actuators[Actuators.size() - 1].X = p_X;
		Actuators[Actuators.size() - 1].Y = p_Y;
		Actuators[Actuators.size() - 1].On_Temp = p_Temp;

		Map[Current_Frame][p_X][p_Y].Type = 3;
		Map[Next_Frame][p_X][p_Y].Type = 3;

		return int(Actuators.size()) - 1;
	}

	int get_Actuator_State(int p_Actuator)
	{
		return Actuators[p_Actuator].is_On();
	}

	float get_Actuator_Temp(int p_Actuator)
	{
		return Actuators[p_Actuator].get_Temp();
	}

	void turn_Actuator_On(int p_Actuator)
	{
		Actuators[p_Actuator].turn_On();
	}

	void turn_Actuator_Off(int p_Actuator)
	{
		Actuators[p_Actuator].turn_Off();
	}
	
	float get_Sensor_Data(int p_Sensor)
	{
		Sensors[p_Sensor].set_Current(Map[Current_Frame][Sensors[p_Sensor].X][Sensors[p_Sensor].Y].Temp);
		return Sensors[p_Sensor].Current;
	}
	
	float get_Sensor_Delta(int p_Sensor)
	{
		return Sensors[p_Sensor].get_Delta();
	}

	void new_Map(int p_Width, int p_Height)
	{
		Width = p_Width;
		Height = p_Height;

		std::cout << "\n Creating Map(" << Width << ", " << Height << ")";

		Map.resize(2);

		Map[0].resize(p_Width);
		Map[1].resize(p_Width);
		for (int cou_X = 0; cou_X < p_Width; cou_X++)
		{
			Map[0][cou_X].resize(p_Height);
			Map[1][cou_X].resize(p_Height);
		}

		std::cout << "\n Finished Map";
	}

	void set_Temp(int p_X, int p_Y, float p_Temp)
	{
		Map[Current_Frame][p_X][p_Y].Temp = p_Temp;
	}

	void set_Map_Temp(float p_Temp)
	{
		for (int cou_X = 0; cou_X < Width; cou_X++)
		{
			for (int cou_Y = 0; cou_Y < Height; cou_Y++)
			{
				set_Temp(cou_X, cou_Y, p_Temp);
			}
		}
	}

	void set_Type(int p_X, int p_Y, int p_Type)
	{
		Map[Current_Frame][p_X][p_Y].Type = p_Type;
		Map[Next_Frame][p_X][p_Y].Type = p_Type;
	}

	bool check_XY(int p_X, int p_Y)
	{
		if (p_X < 0) { return 0; }
		if (p_Y < 0) { return 0; }
		if (p_X >= Width) { return 0; }
		if (p_Y >= Height) { return 0; }

		if (Map[Current_Frame][p_X][p_Y].Type == 0) { return 0; }

		return 1;
	}

	void diffuse_Temp(int p_X, int p_Y)
	{
		float tmp_Total = 0.0;
		float tmp_Count = 0.0;
		float tmp_Current = 0.0;

		if (!check_XY(p_X, p_Y)) { return; }

		//std::cout << "\n C: " << Map[Current_Frame][p_X][p_Y].Temp;
		tmp_Current = Map[Current_Frame][p_X][p_Y].Temp;
		if (check_XY(p_X + 1, p_Y)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X + 1][p_Y].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X, p_Y + 1)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X][p_Y + 1].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X - 1, p_Y)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X - 1][p_Y].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X, p_Y - 1)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X][p_Y - 1].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }

		tmp_Current = Map[Current_Frame][p_X][p_Y].Temp;
		if (check_XY(p_X, p_Y - 1)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X][p_Y - 1].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X + 1, p_Y)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X + 1][p_Y].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X, p_Y + 1)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X][p_Y + 1].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X - 1, p_Y)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X - 1][p_Y].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }

		tmp_Current = Map[Current_Frame][p_X][p_Y].Temp;
		if (check_XY(p_X - 1, p_Y)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X - 1][p_Y].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X, p_Y - 1)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X][p_Y - 1].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X + 1, p_Y)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X + 1][p_Y].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X, p_Y + 1)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X][p_Y + 1].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }

		tmp_Current = Map[Current_Frame][p_X][p_Y].Temp;
		if (check_XY(p_X, p_Y + 1)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X][p_Y + 1].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X - 1, p_Y)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X - 1][p_Y].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X, p_Y - 1)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X][p_Y - 1].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }
		if (check_XY(p_X + 1, p_Y)) { tmp_Current = (tmp_Current + Map[Current_Frame][p_X + 1][p_Y].Temp) / 2; tmp_Count++; tmp_Total += tmp_Current; }

		Map[Next_Frame][p_X][p_Y].Temp = (tmp_Total /= tmp_Count);
		//std::cout << "\n C: " << Map[Current_Frame][p_X][p_Y].Temp;
		//std::cout << "\n N: " << Map[Next_Frame][p_X][p_Y].Temp;
	}

	float get_Temp(int p_X, int p_Y)
	{
		return Map[Current_Frame][p_X][p_Y].Temp;
	}

	void update()
	{
		for (int cou_Index = 0; cou_Index < Actuators.size(); cou_Index++)
		{
			if (Actuators[cou_Index].is_On() == 1)
			{
				Map[Current_Frame][Actuators[cou_Index].X][Actuators[cou_Index].Y].Temp = Actuators[cou_Index].get_Temp();
			}
		}

		for (int cou_X = 0; cou_X < (Width); cou_X++)
		{
			for (int cou_Y = 0; cou_Y < (Height); cou_Y++)
			{
				diffuse_Temp(cou_X, cou_Y);
			}
		}
		swap_Frame();
	}

	void view_Map()
	{
		std::string tmp_Out = "\n";
		for (int cou_X = 0; cou_X < Width; cou_X++)
		{
			tmp_Out += "\n[";
			for (int cou_Y = 0; cou_Y < Height; cou_Y++)
			{
				//std::cout << "[" << Map[Current_Frame][cou_X][cou_Y].Temp << "]";

				if (Map[Current_Frame][cou_X][cou_Y].Type == 0)
				{
					tmp_Out += char(178);
					continue;
				}
				if (Map[Current_Frame][cou_X][cou_Y].Type == 2)
				{
					tmp_Out += 'S';
					continue;
				}
				if (Map[Current_Frame][cou_X][cou_Y].Type == 3)
				{
					tmp_Out += 'A';
					continue;
				}
				if (char(Map[Current_Frame][cou_X][cou_Y].Temp) > 20)
				{
					tmp_Out += char(Map[Current_Frame][cou_X][cou_Y].Temp);
				}
				else
				{
					tmp_Out += " ";
				}
			}
			tmp_Out += "]";
			if (cou_X < Sensors.size())
			{
				tmp_Out += " S[" + std::to_string(cou_X) + "]: [";
				tmp_Out = tmp_Out + "(" + std::to_string(Sensors[cou_X].X) + ", " + std::to_string(Sensors[cou_X].Y) + ") ";
				tmp_Out += "\t" + std::to_string(get_Sensor_Data(cou_X) + 0.000001);
				tmp_Out += "   \tDelta: " + std::to_string(get_Sensor_Delta(cou_X));
				tmp_Out += "] ";
			}
			if (cou_X > Sensors.size())
			{
				int tmp_Index = (cou_X - Sensors.size() - 1);

				if (tmp_Index < Actuators.size())
				{
					tmp_Out += " A[" + std::to_string(tmp_Index) + "]: [";
					if (get_Actuator_State(tmp_Index) == -1) { tmp_Out += " ----[OFF] "; }
					if (get_Actuator_State(tmp_Index) == 0) { tmp_Out += " [BROKEN] "; }
					if (get_Actuator_State(tmp_Index) == 1) { tmp_Out += " [ON]---- "; }
					//tmp_Out += " On/Off: " + std::to_string(get_Actuator_State(tmp_Index));
					tmp_Out += " Temp: " + std::to_string(get_Actuator_Temp(tmp_Index));
					tmp_Out = tmp_Out + "(" + std::to_string(Actuators[tmp_Index].X) + ", " + std::to_string(Actuators[tmp_Index].Y) + ") ";
					tmp_Out += "] ";
				}
			}
		}
		//tmp_Out += "\n";

		std::cout << tmp_Out;
	}

	
	void write_Map_Tiles(std::string p_FName)
	{
		std::ofstream SF;
		std::string tmp_FName = p_FName + ".MAP.dat";
		SF.open(tmp_FName, std::ios::app);

		SF << "\n";
		for (int cou_X = 0; cou_X < Width; cou_X++)
		{
			SF << "\n";
			for (int cou_Y = 0; cou_Y < Height; cou_Y++)
			{
				SF << Map[Current_Frame][cou_X][cou_Y].Temp << " ";
			}
		}
		SF << "\n";
		SF.close();
	}

	void write_Map_Temp(int p_Sensor, std::string p_FName)
	{
		std::ofstream SF;
		std::string tmp_FName = p_FName + "." + std::to_string(p_Sensor) + ".ssv";
		SF.open(tmp_FName, std::ios::app);

		SF << " " << get_Sensor_Data(p_Sensor);

		SF << "\n";
		SF.close();
	}

	void write_Map_Temps(std::string p_FName)
	{
		float tmp_Total = 0;

		for (int cou_S = 0; cou_S < Sensors.size(); cou_S++)
		{
			write_Map_Temp(cou_S, p_FName);
			tmp_Total += get_Sensor_Data(cou_S);
		}

		tmp_Total /= Sensors.size();

		std::ofstream SF;
		std::string tmp_FName = p_FName + ".Temp_Avg.ssv";
		SF.open(tmp_FName, std::ios::app);

		SF << " " << tmp_Total;

		SF << "\n";
		SF.close();
	}

	void write_Map_MSE(int p_Sensor, std::string p_FName)
	{
		std::ofstream SF;
		std::string tmp_FName = p_FName + "." + std::to_string(p_Sensor) + ".ssv";
		SF.open(tmp_FName, std::ios::app);

		SF << " " << ((80 - get_Sensor_Data(p_Sensor)) * (80 - get_Sensor_Data(p_Sensor)));

		SF << "\n";
		SF.close();
	}

	void write_Map_MSE(std::string p_FName)
	{
		float tmp_Total_MSE = 0;
		for (int cou_S = 0; cou_S < Sensors.size(); cou_S++)
		{
			write_Map_MSE(cou_S, p_FName);

			tmp_Total_MSE += ((80 - get_Sensor_Data(cou_S)) * (80 - get_Sensor_Data(cou_S)));
		}

		tmp_Total_MSE /= Sensors.size();

		std::ofstream SF;
		std::string tmp_FName = p_FName + ".MSE_Avg.ssv";
		SF.open(tmp_FName, std::ios::app);

		SF << " " << tmp_Total_MSE;

		SF << "\n";
		SF.close();
	}

	int get_Actuator_Count()
	{
		return Actuators.size();
	}

	int get_Sensor_Count()
	{
		return Sensors.size();
	}

	void draw_Building(int p_X0, int p_Y0, int p_X1, int p_Y1)
	{
		for (int cou_X = p_X0; cou_X <= p_X1; cou_X++)
		{
			set_Type(cou_X, p_Y0, 0);
			set_Type(cou_X, p_Y1, 0);
		}

		for (int cou_Y = p_Y0; cou_Y <= p_Y1; cou_Y++)
		{
			set_Type(p_X0, cou_Y, 0);
			set_Type(p_X1, cou_Y, 0);
		}

		//p_Map->set_Type(23, 20, 1);
	}
};

void draw_Building(c_Map_Sim * p_Map, int p_X0, int p_Y0, int p_X1, int p_Y1)
{
	for (int cou_X = p_X0; cou_X <= p_X1; cou_X++)
	{
		p_Map->set_Type(cou_X, p_Y0, 0);
		p_Map->set_Type(cou_X, p_Y1, 0);
	}

	for (int cou_Y = p_Y0; cou_Y <= p_Y1; cou_Y++)
	{
		p_Map->set_Type(p_X0, cou_Y, 0);
		p_Map->set_Type(p_X1, cou_Y, 0);
	}

	//p_Map->set_Type(23, 20, 1);
}

void runmap()
{
	c_Map_Sim Map(60, 175);

	int tmp_X = 0;
	int tmp_Y = 0;
	float tmp_Temp = 0.0;
	float tmp_Sensor = 0;
	bool flg_Ranran = false;
	bool flg_Heater = false;
	bool flg_AC = false;


	draw_Building(&Map, 10, 10, 40, 40);
	draw_Building(&Map, 10, 45, 40, 75);
	draw_Building(&Map, 10, 80, 40, 110);
	draw_Building(&Map, 10, 115, 40, 145);

	draw_Building(&Map, 9, 9, 46, 146);
	draw_Building(&Map, 40, 10, 45, 145);

	draw_Building(&Map, 46, 10, 56, 20);

	Map.set_Type(11, 40, 1);
	Map.set_Type(11, 75, 1);
	Map.set_Type(11, 110, 1);
	Map.set_Type(11, 145, 1);

	Map.set_Type(39, 40, 1);
	Map.set_Type(39, 75, 1);
	Map.set_Type(39, 110, 1);
	Map.set_Type(39, 145, 1);

	Map.set_Type(11, 45, 1);
	Map.set_Type(11, 80, 1);
	Map.set_Type(11, 115, 1);

	Map.set_Type(9, 25, 1);
	Map.set_Type(10, 25, 1);

	Map.set_Type(40, 41, 1);
	Map.set_Type(40, 42, 1);
	Map.set_Type(40, 43, 1);
	Map.set_Type(40, 44, 1);

	Map.set_Type(40, 76, 1);
	Map.set_Type(40, 77, 1);
	Map.set_Type(40, 78, 1);
	Map.set_Type(40, 79, 1);

	Map.set_Type(40, 111, 1);
	Map.set_Type(40, 112, 1);
	Map.set_Type(40, 113, 1);
	Map.set_Type(40, 114, 1);

	Map.set_Type(39, 146, 1);


	Map.set_Type(45, 111, 1);
	Map.set_Type(45, 112, 1);
	Map.set_Type(45, 113, 1);
	Map.set_Type(46, 112, 1);


	for (int cou_Y = 13; cou_Y < 19; cou_Y++)
	{
		Map.set_Type(45, cou_Y, 1);
		Map.set_Type(46, cou_Y, 1);
	}

	//Map.set_Type(5, 5, 1);
	//Map.set_Type(45, 45, 1);

	Map.add_Sensor(11, 30);
	Map.add_Sensor(11, 65);
	Map.add_Sensor(11, 100);
	Map.add_Sensor(11, 135);

	Map.add_Sensor(47, 11);


	Map.add_Actuator(51, 14, 750);
	Map.add_Actuator(51, 16, 750);

	Map.add_Actuator(50, 90, 0);
	Map.turn_Actuator_On(2);

	//Outside
	Map.add_Actuator(0, 0, 0);
	Map.add_Actuator(0, 25, 0);
	Map.add_Actuator(0, 50, 0);
	Map.add_Actuator(0, 75, 0);
	Map.add_Actuator(0, 100, 0);
	Map.add_Actuator(0, 125, 0);
	Map.add_Actuator(0, 150, 0);
	Map.add_Actuator(0, 174, 0);

	Map.turn_Actuator_On(3);
	Map.turn_Actuator_On(4);
	Map.turn_Actuator_On(5);
	Map.turn_Actuator_On(6);
	Map.turn_Actuator_On(7);
	Map.turn_Actuator_On(8);
	Map.turn_Actuator_On(9);
	Map.turn_Actuator_On(10);

	/*
	//Map.set_Temp(tmp_X, tmp_Y, tmp_Temp);
	for (int cou_Index = 0; cou_Index < 4; cou_Index++)
	{
		for (int cou_Index = 0; cou_Index < 75; cou_Index++)
		{
			Map.set_Temp(24, 24, 0);

			Map.update();
		}
		Map.view_Map();
	}*/

	while (1)
	{
		tmp_X = 0;
		tmp_Y = 0;
		tmp_Temp = 0;

		/*
		std::cout << "Enter Coord:";
		std::cout << "\n X:";
		std::cin >> tmp_X;
		std::cout << " Y:";
		std::cin >> tmp_Y;
		std::cout << " Enter Char: ";

		std::cin >> tmp_Temp;
		*/

		tmp_X = (rand() % 10) + 20;
		tmp_Y = (rand() % 10) + 20;
		tmp_Temp = float((rand() % 2) * 200);

		flg_Ranran = 0;
		//if (!(rand() % 5)) { flg_Ranran = 1; }
		//if (!flg_AC && !flg_Heater) { flg_Ranran = 1; }

		//Map.set_Temp(tmp_X, tmp_Y, tmp_Temp);
		for (int cou_Index = 0; cou_Index < 1; cou_Index++)
		{

			for (int cou_Index = 0; cou_Index < 25; cou_Index++)
			{
				tmp_Sensor = Map.get_Sensor_Data(2);
				if (flg_Ranran) { Map.set_Temp(tmp_X, tmp_Y, tmp_Temp); }

				if (tmp_Sensor > 102) { flg_AC = true; }
				if (tmp_Sensor < 98) { flg_Heater = true; }

				if (tmp_Sensor > 100) { flg_Heater = false; }
				if (tmp_Sensor < 100) { flg_AC = false; }

				if (flg_Heater) 
				{ 
					//Map.set_Temp(45, 45, 225); 
					Map.turn_Actuator_On(0);
					Map.turn_Actuator_On(1);
				}
				if (!flg_Heater) 
				{ 
					//Map.set_Temp(45, 45, 225); 
					Map.turn_Actuator_Off(0);
					Map.turn_Actuator_Off(1);
				}

				/*
				if (flg_AC) 
				{ 
					//Map.set_Temp(5, 5, 0); 
					Map.turn_Actuator_On(2);
					Map.turn_Actuator_On(3);
				}

				if (!flg_AC) 
				{ 
					//Map.set_Temp(5, 5, 0); 
					Map.turn_Actuator_Off(2);
					Map.turn_Actuator_Off(3);
				}*/

				//Map.set_Temp(30, 40, 125);

				Map.update();
			}
			/*
			std::cout << " Temp at Sensor (25, 25): " << tmp_Sensor;
			std::cout << " AC: " << flg_AC;
			std::cout << " Heat: " << flg_Heater;
			*/
			std::cout << "     ";
			std::cout << "     ";
			std::cout << "     ";
			std::cout << "     ";
			std::cout << "     ";
			std::cout << "     ";
			Map.view_Map();
		}
	}

}



















/*
Map.set_Type(23, 40, 1);
Map.set_Type(24, 40, 1);

Map.set_Type(26, 40, 1);
Map.set_Type(27, 40, 1);
*/

/*
for (int cou_X = 40; cou_X <= 50; cou_X++)
{
	Map.set_Type(22, cou_X, 0);

	Map.set_Type(25, cou_X, 0);

	Map.set_Type(28, cou_X, 0);
}

for (int cou_X = 0; cou_X < 10; cou_X++)
{
	Map.set_Type(25, cou_X, 0);
}

for (int cou_X = 30; cou_X < 74; cou_X++)
{
	Map.set_Type(25, cou_X, 0);
}

for (int cou_X = 0; cou_X < 55; cou_X++)
{
	Map.set_Type(cou_X, (cou_X + 70), 0);
}
for (int cou_X = 130; cou_X < 140; cou_X++)
{
	Map.set_Type(15, cou_X, 0);
}
for (int cou_X = 130; cou_X < 140; cou_X++)
{
	Map.set_Type(45, cou_X, 0);
}*/

/*
for (int cou_N = 0; cou_N < 100; cou_N++)
{
	int tmp_XX = (rand() % 30) + 15;
	int tmp_YY = (rand() % 5) + 130;

	Map.set_Type(tmp_XX, tmp_YY, 0);
}*/

/*
for (int cou_X = 0; cou_X < 60; cou_X++)
{
	Map.set_Type(10, cou_X, 0);
}

for (int cou_X = 10; cou_X < 75; cou_X++)
{
	Map.set_Type(20, cou_X, 0);
}

for (int cou_X = 0; cou_X < 60; cou_X++)
{
	Map.set_Type(30, cou_X, 0);
}

for (int cou_X = 10; cou_X < 75; cou_X++)
{
	Map.set_Type(40, cou_X, 0);
}
*/