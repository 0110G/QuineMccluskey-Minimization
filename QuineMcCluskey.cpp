// Quine McCluskey Implementation to Minimize the given MinTerm Expression
// Completed on 17th December, 2018.
// Author : Bhavya Saraf, CSE Sophomore, BITS Pilani, K.K. Birla Goa Campus
#include<bits/stdc++.h>
using namespace std;

//Global Scope Module Declarations
vector<string>minTermsInBits;
vector<string>minTermsInInt;
vector<string>implicantsRecordInBits;
vector<string>implicantsRecord;
vector<bool>trackRecord;
vector<string>essentialPrimeImplicantsInBits;
vector<string>dominatingRowsInBits;
map<string,bool>minTermOccured;

int numberOfMinTerms;
bool arr[50][50];
int presentInFinal[50] = {0};

string ConvertNumberToBits(int minTerm){			// Generates an 8 bit byte string for minTerm
	int num = minTerm;
	int index = 0; 
	char bits[20];
	char rbits[20];
	for (int i=0 ; i<8 ; i++){
		bits[i] = num%2 + '0';
		num/=2;
	}
	bits[8] = '\0';
	for (int i=7 ; i>=0 ; i--){
		rbits[7-i] = bits[i]; 
	}
	rbits[8] = '\0';
	string str = rbits;					// Program not created keeping timeComplexities in mind
	return str;
}

string ConvertNumberToString(int num){				// Generates a string to represent the given number
	int res = num;
	int index = 0;
	char ch[4],rch[4];
	while (res){
		ch[index] = res%10 + '0';
		res/=10;
		index++;
	}
	ch[index] = '\0';
	for (int i=index-1 ; i>=0 ; i--){
		rch[index-1-i] = ch[i];
	}
	rch[index] = '\0';
	if (num == 0){
		rch[0] = '0';
		rch[1] = '\0';
	}
	string str = rch;
	return	str;
}

bool CheckOneBitDifference(string s1, string s2){		// Checks for one bit difference between two given bit strings
	int count = 0;
	for (int i=0 ; i<8 ; i++){
		if (s1[i]!=s2[i]){
			count++;
		}
	}
	return (count==1);
}

string EditByteString(string s1, string s2){			// Returns a string with '-' in the different bit
	string res = s1;
	for (int i=0 ; i<8 ; i++){
		if (s1[i]!=s2[i]){
			res[i] = '-';
		}	
	}
	return res;
}

void MakeTable(int start){					// Generates the MinTerm Table recursively
	bool toRepeat = false;
	int len = minTermsInBits.size();
	for (int i=start ; i<len-1 ; i++){
		for (int j=i+1 ; j<len ; j++){
			if (CheckOneBitDifference(minTermsInBits[i],minTermsInBits[j])){
				trackRecord[i] = true;
				trackRecord[j] = true;
				if (!minTermOccured[EditByteString(minTermsInBits[i],minTermsInBits[j])]){
					minTermsInInt.push_back(minTermsInInt[i]+","+minTermsInInt[j]);
					minTermsInBits.push_back(EditByteString(minTermsInBits[i],minTermsInBits[j]));
					minTermOccured[EditByteString(minTermsInBits[i],minTermsInBits[j])] = true;
					toRepeat = true;
				}
			}			
		}
	}
	if (!toRepeat){
		return;
	}
	else{
		MakeTable(len);
	}
}

void Print(){							// Print the neccessary information for testing purposes
	int len = minTermsInBits.size();
	for (int i=0 ; i<len ; i++){
		cout << minTermsInInt[i] << "\t\t" << minTermsInBits[i] << "\t" << trackRecord[i] << "\n";
	}
	//cout << minTermsInInt.size() << minTermsInBits.size() << trackRecord.size();		#Test
}

void ImplicantTable(){						// Generates the implicants record and implication table
	int len = minTermsInInt.size();
	for (int i=0 ; i<len ; i++){
		if (!trackRecord[i]){				// Generates the Vector array implementRecords.
			implicantsRecord.push_back(minTermsInInt[i]);
			implicantsRecordInBits.push_back(minTermsInBits[i]);
		}
	}
	
	len = implicantsRecord.size();
	for (int i=0 ; i<len ; i++){
		for (int j=0 ; j<numberOfMinTerms ; j++){
			arr[i][j] = false;
			char temp1[30], temp2[10];		//Requires sorted input implicantsRecord and 
			strcpy(temp1,implicantsRecord[i].c_str());
			strcpy(temp2,minTermsInInt[j].c_str());
			char *ptr = strstr(temp1,temp2); 
			if (ptr && (temp1[ptr-temp1+strlen(temp2)]==',' || temp1[ptr-temp1+strlen(temp2)]=='\0')){
				arr[i][j] = true;
			}
			//cout << arr[i][j] << " ";		// #Test
		}
		//cout << "\n";   	                        // #Test
	}
	
}

void EssentialPrimeImplicants(){
	int len = implicantsRecord.size();
	int count = 0;
	int index = -1;
	for (int i=0 ; i<numberOfMinTerms ; i++){
		count = 0;
		for (int j=0 ; j<len ; j++){
			if (arr[j][i]){
				count++;
				index = j;
			}
		}
		if (count==1){
			essentialPrimeImplicantsInBits.push_back(implicantsRecordInBits[index]);
			presentInFinal[index] = 1;
			for (int k=0 ; k<numberOfMinTerms ; k++){
				if (arr[index][k]){
					for (int l=0 ; l<len ; l++){
						arr[l][k] = false;
					}
				}
			}
		}
	}
}

void RemoveDominatedRows(){						// To be executed after EssentialPrimeImplicants()
	int len = implicantsRecord.size();
	bool dominated = true;
	bool rDominated = true;
	for (int i=0 ; i<len ; i++){					// Column j dominated by i
		for (int j=0 ; j<len ; j++){
			if (presentInFinal[j]==0){
				dominated = true;
				rDominated = true;
				if (i!=j){
					for (int k=0 ; k<numberOfMinTerms ; k++){
						if (arr[j][k] != arr[i][k]){
							rDominated = false;			
						}
						if (arr[j][k]==1 && arr[i][k]!=1){
							dominated = false;
							break;
						}
					}
					if (dominated && presentInFinal[j]==0){
						presentInFinal[j] = -1;
					}
					if (rDominated){
						presentInFinal[i] = 2;				// Codes: 0 is used if implicant is unsure of
					}							// 1 is used if it is a essential prime
				}								// implicant.
			}									// 2 is used if two of the prime implicants
		}										// have exactly same configuration in arr[][]
	}											// after removing the essential prime
}												// implicants.
												// -1 is used if the implicant is row 
void PrintFinalResult(int numberOfBits){							// dominated
	int len = implicantsRecord.size();
	char ch = 'A';
	for (int i=0 ; i<len ; i++){
		if (presentInFinal[i]!=-1){							// Prints the final human readable result
			ch = 'A';
			for (int j=8-numberOfBits ; j<8 ; j++){
				if (implicantsRecordInBits[i][j]=='0'){
					cout << ch << "'";
				}
				if (implicantsRecordInBits[i][j]=='1'){
					cout << ch;
				}
				ch++;	
			}
			cout << " + ";	
		}
	}
	cout << "\n";					
}

int main(){

	int numberOfBits;
	
	
	cout << "Enter the number of bits: ";
	cin >> numberOfBits;
	
	cout << "Enter the number of MinTerms: ";
	cin >> numberOfMinTerms;

	int minTerms[numberOfMinTerms];
	
	for (int i=0 ; i<numberOfMinTerms ; i++){
		cout << "Enter MinTerm: ";
		cin >> minTerms[i];
	}

	sort(minTerms,minTerms+numberOfMinTerms);						// Sorting neccessary for -	
												// ImplicantTable() to work
	for (int i=0 ; i<numberOfMinTerms ; i++){
		minTermsInBits.push_back(ConvertNumberToBits(minTerms[i]));
		minTermsInInt.push_back(ConvertNumberToString(minTerms[i]));
		trackRecord.push_back(false);
	}
	MakeTable(0);
	//Print();										// The status after MakeTable()
	//cout << "\n";
	ImplicantTable();									// Essential Prime Implicants found!
	EssentialPrimeImplicants();
	//cout << "\n";
	RemoveDominatedRows();									// Dominated Rows Removed!
	PrintFinalResult(numberOfBits);	
	/*cout << "\n\n";
	int len = implicantsRecord.size();
	for (int i=0 ; i<len ; i++){
		for (int j=0 ; j<numberOfMinTerms ; j++){
			cout << arr[i][j] << " ";
		}
		cout << implicantsRecord[i] << " " << implicantsRecordInBits[i] << ":" << presentInFinal[i] << "\n";
	}*/
	return (0);	
}
