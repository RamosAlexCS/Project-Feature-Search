#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

void PrintMenu();
void Algorithm_Method();
vector<vector<float>> readFile(string);
vector<vector<float>> convertStringToFloat(vector<vector<string>>);
void feature_search(vector<vector<float>>);
void back_search(vector<vector<float>>);
double forward_acc(vector<vector<float>>, vector<int>, int);
double back_acc(vector<vector<float>>, vector<int>, int);
void pringResults(vector<double>, vector<vector<int>>);

string Method;

int main() {
	
	PrintMenu();

	return 0;
}


//Menu
void PrintMenu() {
	string menuOp = "\0";

	//Output menu option, prompt users for valid selection
	while (menuOp != "Q") {
		vector<vector<float>> data;
		Method = "\0";
		cout << "\nSELECT FILE MENU" << endl;
		cout << "1 - CS170_Small_Data__56.txt" << endl;
		cout << "2 - CS170_Large_Data__65.txt" << endl;
		cout << "Q - Quit" << endl << endl;
		cout << "Choose an option:" << endl;
		cin >> menuOp;

		while (menuOp != "1" && menuOp != "2" && menuOp != "Q") {
			cout << "Invalid Input, Try again.\n";
			cin >> menuOp;
		}

		char Op = menuOp.at(0);

		//Call corresponding menu action
		switch (Op) {
		case '1':
			data = readFile(menuOp);
			while (Method != "Q") {
				Algorithm_Method();

				if (Method == "1") {
					feature_search(data);
				}
				else if (Method == "2") {
					back_search(data);
				}
			}

			break;
		case '2':
			data = readFile(menuOp);
			while (Method != "Q") {
				Algorithm_Method();

				if (Method == "1") {
					feature_search(data);
				}
				else if (Method == "2") {
					back_search(data);
				}

			}

			break;
		}
	}
}

//Picking which algorithm to use to solve the puzzle
void Algorithm_Method() {
	Method = '\0';
	cout << "Which algorithm would you like to use to solve the current file : \n";
	cout << "1 - Feature Search\n";
	cout << "2 - Backwards Elimination\n";
	cout << "Q - go back to menu\n";
	cin >> Method;
	while (Method != "1" && Method != "2" && Method != "Q") {
		cout << "Invalid Input, Try again.\n";
		cin >> Method;
	}
}

//Read from file
vector<vector<float>> readFile(string file) {
	vector<string> lineRead;
	vector<vector<string>> fileRead;
	vector<vector<float>> fileReady;
	string myText;

	if (file == "1") {
		file = "CS170_Small_Data__56.txt";
	}
	else if (file == "2") {
		file = "CS170_Large_Data__65.txt";
	}

	ifstream MyReadFile(file);
	
	int check = 1;
	while (!MyReadFile.eof()) {
		while (getline(MyReadFile, myText)) {
			if (!myText.empty()) {
				int start = 0;
				int length = 0;
				do {
					if (start > myText.length() - 1) {
						break;
					}
					do {
						if (myText.at(start) != ' ') {
							length = start + 1;
							break;
						}
						else {
							start++;
						}
					} while (true);

					do {
						if (myText.at(length) == ' ' || length >= myText.length() - 1) {
							break;
						}
						else {
							length++;
						}
					} while (true);

					if (length >= myText.length() - 1) {
						break;
					}
					lineRead.push_back(myText.substr(start, length - start));
					start = length + 1;
				} while (true);
				lineRead.push_back(myText.substr(start));
			}
			fileRead.push_back(lineRead);
			lineRead.clear();
		}

		MyReadFile.close();
	}

	fileReady = convertStringToFloat(fileRead);

	return fileReady;
}

//Convert string to float
vector<vector<float>> convertStringToFloat(vector<vector<string>> convert) {
	vector<float> convertedLine;
	vector<vector<float>> convertedFile;

	for (int i = 0; i < convert.size(); i++) {
		for (int j = 0; j < convert.at(i).size(); j++) {
			convertedLine.push_back(atof(convert.at(i).at(j).c_str()));
		}
		convertedFile.push_back(convertedLine);
		convertedLine.clear();
	}

	return convertedFile;
}

//forward search
void feature_search(vector<vector<float>> data) {
	time_t start;
	start = time(NULL);

	vector<int> curr_set_feat;
	vector<vector<int>> set_feat_store;
	vector<double> acc_of_set_feat;

	for (int i = 1; i < data.at(0).size(); i++) {
		cout << "On the " << i << "th level of the search tree\n";
		int feat_to_add_to_lvl = 0;
		double best_acc_so_far = 0;

		for (int j = 1; j < data.at(0).size(); j++) {
			bool duplicate = false;
			for (int k = 0; k < curr_set_feat.size(); k++) {
				if (curr_set_feat.at(k) == j) {
					duplicate = true;
					break;
				}
			}

			if (!duplicate) {
				cout << "--Considering adding the " << j << " feature\n";
				double acc = forward_acc(data, curr_set_feat, j);
				cout << "ACC: " << acc << endl;
				cout << "Best acc: " << best_acc_so_far << endl;

				if (acc > best_acc_so_far) {
					best_acc_so_far = acc;
					feat_to_add_to_lvl = j;
				}
			}
		}

		curr_set_feat.push_back(feat_to_add_to_lvl);
		cout << "On level " << i << " i added feature " << feat_to_add_to_lvl << " to current set\n";
		acc_of_set_feat.push_back(best_acc_so_far);
		set_feat_store.push_back(curr_set_feat);
	}

	time_t current_time;
	current_time = time(NULL);
	int time_passed = current_time - start;

	pringResults(acc_of_set_feat, set_feat_store);

	int min = time_passed / 60;
	int sec = time_passed % 60;
	cout << "It took " << min << " minute(s) and " << sec << " second(s) to solve this puzzle" << endl << endl;
}

//foward accuracy
double forward_acc(vector<vector<float>> data, vector<int> curr_set, int feat_to_add) {
	double num_corr_class = 0;

	for (int i = 0; i < data.size(); i++) {
		float  obj_to_class = data.at(i).at(feat_to_add);
		int label_obj_to_class = data.at(i).at(0);

		double near_neighbor_dist = 9999999;
		double near_neighbor_loc = 9999999;
		int near_neighbor_label = 0;
		for (int j = 0; j < data.size(); j++) {
			if (j != i) {
				double dist = 0.0;
				double store = 0.0;
				dist = pow(obj_to_class - data.at(j).at(feat_to_add), 2);
				for (int k = 0; k < curr_set.size(); k++) {
					dist += pow(data.at(i).at(curr_set.at(k)) - data.at(j).at(curr_set.at(k)), 2);
				}
				dist = sqrt(dist);

				if (dist < near_neighbor_dist) {
					near_neighbor_dist = dist;
					near_neighbor_loc = j;
					near_neighbor_label = data.at(near_neighbor_loc).at(0);
				}
			}
		}
		if (label_obj_to_class == near_neighbor_label) {
			num_corr_class++;
		}
	}

	double acc = num_corr_class / data.size();
	return acc;
}

//backward elimination
void back_search(vector<vector<float>> data) {
	time_t start;
	start = time(NULL);

	vector<int> curr_set_feat;
	for (int i = 1; i < data.at(0).size(); i++) {
		curr_set_feat.push_back(i);
	}
	vector<vector<int>> set_feat_store;
	set_feat_store.push_back(curr_set_feat);
	vector<double> acc_of_set_feat;

	for (int i = 1; i < data.at(0).size(); i++) {
		cout << "On the " << i << "th level of the search tree\n";
		int feat_to_remove_to_lvl = 0;
		int pot_feat_to_remove = 0;
		double best_acc_so_far = 0;

		for (int j = 1; j < data.at(0).size(); j++) {
			bool found = false;
			for (int k = 0; k < curr_set_feat.size(); k++) {
				if (curr_set_feat.at(k) == j) {
					found = true;
					break;
				}
			}
			if (found) {
				cout << "--Considering remove the " << j << " feature\n";
				double acc = back_acc(data, curr_set_feat, j);
				cout << "ACC: " << acc << endl;
				cout << "Best acc: " << best_acc_so_far << endl;

				if (acc > best_acc_so_far) {
					best_acc_so_far = acc;
					feat_to_remove_to_lvl = j;
				}
			}
		}

		remove(curr_set_feat.begin(), curr_set_feat.end(), feat_to_remove_to_lvl);
		curr_set_feat.resize(curr_set_feat.size() - 1);
		cout << "On level " << i << " i remove feature " << feat_to_remove_to_lvl << " to current set\n";
		acc_of_set_feat.push_back(best_acc_so_far);
		set_feat_store.push_back(curr_set_feat);
	}

	time_t current_time;
	current_time = time(NULL);
	int time_passed = current_time - start;

	pringResults(acc_of_set_feat, set_feat_store);

	int min = time_passed / 60;
	int sec = time_passed % 60;
	cout << "It took " << min << " minute(s) and " << sec << " second(s) to solve this puzzle" << endl << endl;
}

//backward accuracy
double back_acc(vector<vector<float>> data, vector<int> curr_set, int feat_to_rem) {
	double num_corr_class = 0;

	for (int i = 0; i < data.size(); i++) {
		int label_obj_to_class = data.at(i).at(0);

		double near_neighbor_dist = 9999999;
		double near_neighbor_loc = 9999999;
		int near_neighbor_label = 0;
		for (int j = 0; j < data.size(); j++) {
			if (j != i) {
				double dist = 0.0;
				double store = 0.0;
				for (int k = 0; k < curr_set.size(); k++) {
					if (curr_set.at(k) != feat_to_rem) {
						dist += pow(data.at(i).at(curr_set.at(k)) - data.at(j).at(curr_set.at(k)), 2);
					}
				}
				dist = sqrt(dist);

				if (dist < near_neighbor_dist) {
					near_neighbor_dist = dist;
					near_neighbor_loc = j;
					near_neighbor_label = data.at(near_neighbor_loc).at(0);
				}
			}
		}
		if (label_obj_to_class == near_neighbor_label) {
			num_corr_class++;
		}
	}

	double acc = num_corr_class / data.size();
	return acc;
}

//Print that path taken with set of features and corresponding accuract
//Then prints the best set of features at the end
void pringResults(vector<double> set_acc, vector<vector<int>> set_feat) {

	cout << "-------------------------------------------------\n";
	cout << "*                    DONE                       *\n";
	cout << "-------------------------------------------------\n";

	double high_acc = set_acc.at(0);
	int best_ind = 0;

	for (int i = 0; i < set_acc.size(); i++) {
		cout << "Set feature ";
		for (int j = 0; j < set_feat.at(i).size(); j++) {
			cout << set_feat.at(i).at(j) << " ";
		}
		cout << "has an accuracy of " << (set_acc.at(i) * 100) << "%\n";
		if (high_acc < set_acc.at(i)) {
			high_acc = set_acc.at(i);
			best_ind = i;
		}
	}
	cout << "-------------------------------------------------\n";
	cout << "*                    Best                       *\n";
	cout << "-------------------------------------------------\n";

	cout << "Best feature(s) to add\n";
	for (int i = 0; i < set_feat.at(best_ind).size(); i++) {
		cout << set_feat.at(best_ind).at(i) << " ";
	}
	cout << "\nWith an accuracy of " << (set_acc.at(best_ind) * 100) << "%\n";
}
