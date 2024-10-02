#include<iostream>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#define e 2.71828
#define LEARNING_TIMES 10
#define TRAIN_DATA_FILE "train_data_A.csv"
#define TEST_DATA_FILE "test_data_A.csv"
using namespace std;

struct data_set{
	int pregnancies;
	double glucose;
	double blood_pressure;
	double skin_thickness;
	double insulin;
	double BMI;
	double diabetes_function;
	int age;
	int outcome;
};
vector<data_set> read_data(string filename);

int main()
{
	vector<data_set> train_data;
	vector<data_set> test_data;
	train_data=read_data(TRAIN_DATA_FILE);
	test_data=read_data(TEST_DATA_FILE);
	
	//calculate average
	data_set average={0,0,0,0,0,0,0,0,0},SD={0,0,0,0,0,0,0,0,0};
	int nonzero_gl=0,nonzero_bp=0,nonzero_st=0,nonzero_bmi=0,nonzero_df=0,nonzero_age=0;
	for(int i=0;i<train_data.size();i++)
	{
		average.pregnancies+=train_data[i].pregnancies;
		if(train_data[i].glucose!=0)
		{
			average.glucose+=train_data[i].glucose;
			nonzero_gl++;
		}
		if(train_data[i].blood_pressure!=0)
		{
			average.glucose+=train_data[i].glucose;average.blood_pressure+=train_data[i].blood_pressure;
			nonzero_bp++;
		}
		if(train_data[i].skin_thickness!=0)
		{
			average.skin_thickness+=train_data[i].skin_thickness;
			nonzero_st++;
		}
		average.insulin+=train_data[i].insulin;
		if(train_data[i].BMI!=0)
		{
			average.BMI+=train_data[i].BMI;
			nonzero_bmi++;
		}
		if(train_data[i].diabetes_function!=0)
		{
			average.diabetes_function+=train_data[i].diabetes_function;
			nonzero_df++;
		}
		if(train_data[i].age!=0)
		{
			average.age+=train_data[i].age;
			nonzero_age++;
		}
	}
	average.pregnancies=average.pregnancies/train_data.size();
	average.glucose=average.glucose/nonzero_gl;
	average.blood_pressure=average.blood_pressure/nonzero_bp;
	average.skin_thickness=average.skin_thickness/nonzero_st;
	average.insulin=average.insulin/train_data.size();
	average.BMI=average.BMI/nonzero_bmi;
	average.diabetes_function=average.diabetes_function/nonzero_df;
	average.age=average.age/nonzero_age;
	
	//calculate standard deviation
	for(int i=0;i<train_data.size();i++)
	{
		if(train_data[i].glucose==0) train_data[i].glucose=average.glucose;
		if(train_data[i].blood_pressure==0) train_data[i].blood_pressure=average.blood_pressure;
		if(train_data[i].skin_thickness==0) train_data[i].skin_thickness=average.skin_thickness;
		if(train_data[i].BMI==0) train_data[i].BMI=average.BMI;
		
		SD.pregnancies+=pow(train_data[i].pregnancies-average.pregnancies,2);
		SD.glucose+=pow(train_data[i].glucose-average.glucose,2);
		SD.blood_pressure+=pow(train_data[i].blood_pressure-average.blood_pressure,2);
		SD.skin_thickness+=pow(train_data[i].skin_thickness-average.skin_thickness,2);
		SD.insulin+=pow(train_data[i].insulin-average.insulin,2);
		SD.BMI+=pow(train_data[i].BMI-average.BMI,2);
		SD.diabetes_function+=pow(train_data[i].diabetes_function-average.diabetes_function,2);
		SD.age+=pow(train_data[i].age-average.age,2);
	}
	SD.pregnancies=sqrt(SD.pregnancies/train_data.size());
	SD.glucose=sqrt(SD.glucose/train_data.size());
	SD.blood_pressure=sqrt(SD.blood_pressure/train_data.size());
	SD.skin_thickness=sqrt(SD.skin_thickness/train_data.size());
	SD.insulin=sqrt(SD.insulin/train_data.size());
	SD.BMI=sqrt(SD.BMI/train_data.size());
	SD.diabetes_function=sqrt(SD.diabetes_function/train_data.size());
	SD.age=sqrt(SD.age/train_data.size());
	
	//standardize train data
	for(int i=0;i<train_data.size();i++)
	{
		train_data[i].pregnancies=(train_data[i].pregnancies-average.pregnancies)/SD.pregnancies;
		train_data[i].glucose=(train_data[i].glucose-average.glucose)/SD.glucose;
		train_data[i].blood_pressure=(train_data[i].blood_pressure-average.blood_pressure)/SD.blood_pressure;
		train_data[i].skin_thickness=(train_data[i].skin_thickness-average.skin_thickness)/SD.skin_thickness;
		train_data[i].insulin=(train_data[i].insulin-average.insulin)/SD.insulin;
		train_data[i].BMI=(train_data[i].BMI-average.BMI)/SD.BMI;
		train_data[i].diabetes_function=(train_data[i].diabetes_function-average.diabetes_function)/SD.diabetes_function;
		train_data[i].age=(train_data[i].age-average.age)/SD.age;
	}
	
	//standardize test data
	for(int i=0;i<test_data.size();i++)
	{
		test_data[i].pregnancies=(test_data[i].pregnancies-average.pregnancies)/SD.pregnancies;
		test_data[i].glucose=(test_data[i].glucose-average.glucose)/SD.glucose;
		test_data[i].blood_pressure=(test_data[i].blood_pressure-average.blood_pressure)/SD.blood_pressure;
		test_data[i].skin_thickness=(test_data[i].skin_thickness-average.skin_thickness)/SD.skin_thickness;
		test_data[i].insulin=(test_data[i].insulin-average.insulin)/SD.insulin;
		test_data[i].BMI=(test_data[i].BMI-average.BMI)/SD.BMI;
		test_data[i].diabetes_function=(test_data[i].diabetes_function-average.diabetes_function)/SD.diabetes_function;
		test_data[i].age=(test_data[i].age-average.age)/SD.age;
	}
	
	//training
	double learning_rate=0.01,prediction,k;
	double b[9]={};
	for(int j=0;j<LEARNING_TIMES;j++)
	{
		for(int i=0;i<train_data.size();i++)
		{
			k=b[0]+b[1]*train_data[i].pregnancies+b[2]*train_data[i].glucose
			+b[3]*train_data[i].blood_pressure+b[4]*train_data[i].skin_thickness
			+b[5]*train_data[i].insulin+b[6]*train_data[i].BMI
			+b[7]*train_data[i].diabetes_function+b[8]*train_data[i].age;
			prediction=1/(1+pow(e,0-k));
			b[0]+=learning_rate*(train_data[i].outcome-prediction)*prediction*(1-prediction)*1.0;
			b[1]+=learning_rate*(train_data[i].outcome-prediction)*prediction*(1-prediction)*train_data[i].pregnancies;
			b[2]+=learning_rate*(train_data[i].outcome-prediction)*prediction*(1-prediction)*train_data[i].glucose;
			b[3]+=learning_rate*(train_data[i].outcome-prediction)*prediction*(1-prediction)*train_data[i].blood_pressure;
			b[4]+=learning_rate*(train_data[i].outcome-prediction)*prediction*(1-prediction)*train_data[i].skin_thickness;
			b[5]+=learning_rate*(train_data[i].outcome-prediction)*prediction*(1-prediction)*train_data[i].insulin;
			b[6]+=learning_rate*(train_data[i].outcome-prediction)*prediction*(1-prediction)*train_data[i].BMI;
			b[7]+=learning_rate*(train_data[i].outcome-prediction)*prediction*(1-prediction)*train_data[i].diabetes_function;
			b[8]+=learning_rate*(train_data[i].outcome-prediction)*prediction*(1-prediction)*train_data[i].age;
		}
	}
	
	//predicting
	double accurarcy;
	int diabetes_prediction,TP=0,FN=0,FP=0,TN=0;
	for(int i=0;i<test_data.size();i++)
	{
		k=b[0]+b[1]*test_data[i].pregnancies+b[2]*test_data[i].glucose
		+b[3]*test_data[i].blood_pressure+b[4]*test_data[i].skin_thickness
		+b[5]*test_data[i].insulin+b[6]*test_data[i].BMI
		+b[7]*test_data[i].diabetes_function+b[8]*test_data[i].age;
		k=exp(k)/(1+exp(k));
		if(k>0.5) diabetes_prediction=1;
		else diabetes_prediction=0;
		if(diabetes_prediction==test_data[i].outcome)
		{
			if(diabetes_prediction==1) TP++;
			else if(diabetes_prediction==0) TN++;
		}
		else
		{
			if(diabetes_prediction==1) FP++;
			else if(diabetes_prediction==0) FN++;
		}
	}
	
	int P=TP+FN,N=FP+TN;
	accurarcy=(double)(TP+TN)/test_data.size();
	cout<<"\n\tTrue Positive: "<<TP<<"\tFalse Negative: "<<FN<<"\tTotal Positive: "<<P<<endl
	<<"\tFalse Positive: "<<FP<<"\tTrue Negative: "<<TN<<"\tTotal Negative: "<<N<<endl;
	cout<<"\tAccuracy: "<<accurarcy<<endl;
	cout<<"\tError Rate: "<<1-accurarcy<<endl;
	cout<<"\tSensitivity: "<<(double)TP/P<<endl;
	cout<<"\tSpecificity:"<<(double)TN/N<<endl;
	cout<<"\tPrecision:"<<(double)TP/(TP+FP)<<endl;
	cout<<"\tRecall:"<<(double)TP/(TP+FN)<<endl;
	return 0;
}

vector<data_set> read_data(string filename)
{
	fstream file;
	vector<data_set> data;
	file.open(filename.c_str(),ios::in);
	string line_train;
	data_set data_train;
	getline(file,line_train);//don't want the titles
	while(getline(file,line_train,'\n'))//read train_data.csv file
	{
		istringstream tmp_line(line_train);
		string str;
		while(getline(tmp_line,str,','))
		{
			data_train.pregnancies = atoi(str.c_str());//convert string to int and store

			if(getline(tmp_line,str,','))
				data_train.glucose = atof(str.c_str());//convert string to double and store

			if(getline(tmp_line,str,','))
				data_train.blood_pressure = atof(str.c_str());

			if(getline(tmp_line,str,','))
				data_train.skin_thickness = atof(str.c_str());

			if(getline(tmp_line,str,','))
				data_train.insulin = atof(str.c_str());

			if(getline(tmp_line,str,','))
				data_train.BMI = atof(str.c_str());

			if(getline(tmp_line,str,','))
				data_train.diabetes_function = atof(str.c_str());

			if(getline(tmp_line,str,','))
				data_train.age = atoi(str.c_str());

			if(getline(tmp_line,str,','))
				data_train.outcome = atoi(str.c_str());
		}
		data.push_back(data_train);
	}
	file.close();
	return data;
}
