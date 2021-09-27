#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>
#include <ctime>

using namespace std;

// --- HELPER FUNCTIONS ---
double probCalc(string kmer, double** profile)
{
	// multiply the the value of each char from its corresponding profile index
	double prob = 1;
	for (int i = 0; i < kmer.length(); i++)
	{
		if (kmer[i] == 'A')
			prob *= profile[0][i];
		else if (kmer[i] == 'C')
			prob *= profile[1][i];
		else if (kmer[i] == 'G')
			prob *= profile[2][i];
		else if (kmer[i] == 'T')
			prob *= profile[3][i];
	}
	return prob;
}
void kmerVariations(char set[], vector<string>& list, string kmer, int k)
{
	// recursive function to get all the variations of a k-length k-mer
	// for brute force: (i.e. from AAA to TTT)
	// will probably be removed; too much space for longer k-lengths
	if (k == 0)
	{
		list.push_back(kmer);
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		string newKmer;
		newKmer = kmer + set[i];
		kmerVariations(set, list, newKmer, k - 1);
	}
}
double factorial(int n)
{
	double fct = 1;
	for (int i = 1; i <= n; i++)
		fct *= (double) i;
	return fct;
}

// --- DNA GENERATOR ---
void dnaGenerator(int t, int n, int k, int d, string file)
{
	// initialize alphabet and dna string
	char alphabet[4] = { 'A', 'T', 'C', 'G' };
	string dna = "";

	// create a base motif
	string baseMotif = "";
	for (int i = 0; i < k; i++)
		baseMotif += alphabet[rand() % 4];

	cout << " Base Motif: " << baseMotif << endl;

	int count = 0;
	while (count < t)
	{
		// create randomized n length dna string
		for (int i = 0; i < n; i++)
			dna += alphabet[rand() % 4];

		// alter the base motif with up to d mutations
		string mutatedMotif = baseMotif;
		for (int i = 0; i < d; i++)
			mutatedMotif[rand() % k] = alphabet[rand() % 4];

		// insert mutated motif into dna string
		int index = rand() % (dna.length() - mutatedMotif.length() + 1);
		dna.replace(index, mutatedMotif.length(), mutatedMotif);

		// place into fasta file
		ofstream dnaStrings(file, ios_base::app);
		dnaStrings << ">seq" << (count + 1) << "\n" << dna << "\n";
		dna = "";
		count++;
	}
}

// --- ALGORITHMS --- 
void bruteForceMotif(string file, int k, int d)
{
	// vector to hold all the dna strings
	vector<string> dnaStrings;
	// vector to hold the final motifs
	vector<string> motifs;
	

	string filePath = "../Assignment 2/" + file +".fasta";
	string line;
	// add the dna sequences to a vector list
	ifstream fastaFile(filePath);
	while (getline(fastaFile, line))
	{
		// skip the identifier and add the dna string
		getline(fastaFile, line);
		dnaStrings.push_back(line);
	}


	// iterate through every kmer in all dna strings
	for (int i = 0; i < dnaStrings.size(); i++)
	{
		// loop through all the kmers in a single dna string
		int kmerSubstr = 0;
		vector<pair<string, int>> minScores;
		while (kmerSubstr <= dnaStrings[i].size() - k)
		{
			string kmer = dnaStrings[i].substr(kmerSubstr, k);
			set<int> kmerScores;

			// loop through every motif in all dna strings
			int motifSubstr = 0;
			for (int j = 0; j < dnaStrings.size(); j++)
			{
				// skip over the kmer's own dna string and exit if last string
				if (j == i)
				{
					j++;
					if (j >= dnaStrings.size())
						break;
				}

				// compare each char at each motif and kmer and get a difference
				while (motifSubstr <= dnaStrings[j].size() - k)
				{
					string motif = dnaStrings[j].substr(motifSubstr, k);
					int diff = 0;
					int itr = 0;
					while (itr < motif.length())
					{
						if (kmer[itr] != motif[itr])
							diff++;
						itr++;
					}
					// add the difference score to a set
					kmerScores.insert(diff);
					motifSubstr++;
				}
			}
			// take the minimum difference score and push it to a vector
			int score = *kmerScores.begin();
			minScores.push_back(make_pair(kmer, score));

			kmerSubstr++;
		}
		// find the motif with the smallest difference among the dna string
		// and push it to the final motifs vector
		int index = 0;
		for (int l = 0; l < minScores.size()-1; l++)
		{
			if (minScores[l + 1].second < minScores[l].second)
				index = l + 1;
		}
		motifs.push_back(minScores[index].first);
	}

	ofstream motifFile("a2_2_abraham-kukkadam.o2", ios_base::app);
	for (int i = 0; i < motifs.size(); i++)
		motifFile << motifs[i] << "\n\n";
}
void gibbsSampling(string file, int k, int d)
{
	vector<string> dnaStrings;
	// A 'motifs' vector to do the calculations
	vector<pair<string, double>> motifs;
	// a 'final motifs' vector to hold the final results
	vector<pair<string, double>> finalMotifs;
	long count = 0.0;
	
	string filePath = "../Assignment 2/" + file +".fasta";
	string line;
	// add the dna sequences to a vector list
	ifstream fastaFile(filePath);
	while (getline(fastaFile, line))
	{
		// skip the identifier and add the dna string
		getline(fastaFile, line);
		dnaStrings.push_back(line);
	}
	

	// randomly select k-mer from each dna sequence
	for (int i = 0; i < dnaStrings.size(); i++)
	{
		int index = rand() % (dnaStrings[i].length() - k + 1);
		motifs.push_back(make_pair(dnaStrings[i].substr(index, k), 0));
		finalMotifs.push_back(make_pair(dnaStrings[i].substr(index, k), 0));
	}

	// Loop for the randomized picks
	while (true) 
	{
		// create a minimum passing score for the final motifs to break out of the loop
		// the score of all the final motifs have to be greater than the minScore value
		// -----------------------------------------------------------------------------
		// Equation: (k-1)! / (2(t-1))^k	or		(n-k+1)^t / (2(t-1))^k
		// -----------------------------------------------------------------------------
		// Numerator: an equation of variables as the total number of probabilities
		// [most likely a convoluted and inaccurate way to calculate the minimum score]
		// Denominator: multiplied to account for double observation from laplace's
		// and raised to the number of variations (k)
		
		// * variable declarations *
		int len = dnaStrings[0].length(); int t = motifs.size(); 
		double fct = factorial(k - 1); double prob = pow((len - k + 1), t);

		double minScore = fct / pow(2*(t-1), k);
		
		
		// randomly remove a k-mer from the motifs
		int removed = rand() % motifs.size();
		string removedDNA = dnaStrings[removed];
		motifs.erase(motifs.begin() + removed);


		// create dynamic 2d arry for profile
		int m = 4; int n = k;
		double** profile = new double* [m];
		for (int i = 0; i < m; i++)
			profile[i] = new double[n];
		// initialize profile matrix to ones (pseudocounts / laplace's)
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
				profile[i][j] = 1;
		}

		// create the motif matrix based on each char in the k-mer
		for (int i = 0; i < motifs.size(); i++)
		{
			for (int j = 0; j < motifs[i].first.length(); j++)
			{
				char base = motifs[i].first[j];
				if (base == 'A')
					profile[0][j] += 1;
				else if (base == 'C')
					profile[1][j] += 1;
				else if (base == 'G')
					profile[2][j] += 1;
				else if (base == 'T')
					profile[3][j] += 1;
			}
		}


		// Calculate the probability of the k-mers in the deleted string
		vector<pair<string, double>> seqProb;
		vector<double> probDist;
		double profileTotal = 0;
		for (int i = 0; i <= (removedDNA.length() - k); i++)
		{
			string kmer = removedDNA.substr(i, k);
			double profileScore = probCalc(kmer, profile);
			seqProb.push_back(make_pair(kmer, profileScore));
			profileTotal += profileScore;
		}
		// create a cummulative frequency distribution to choose a k-mer
		double freqProb = 0;
		for (int i = 0; i < seqProb.size(); i++)
		{
			freqProb += (seqProb[i].second / profileTotal);
			probDist.push_back(freqProb);
		}
		// choose a random probability and get the k-mer from the frequency table
		double randProb = (double) rand() / RAND_MAX;
		int index = 0;
		for (int i = 0; i < probDist.size(); i++)
		{
			if (!(randProb > probDist[i]))
			{
				index = i;
				break;
			}
		}
		// variables to hold the chosen k-mer and its score
		string newMotif = seqProb[index].first;
		double motifScore = (seqProb[index].second) / (pow((motifs.size() * 2), k));
		
		// reinsert the chosen k-mer back into the motif list
		motifs.insert(motifs.begin() + removed, make_pair(newMotif, motifScore));

		// Calculate consensus motifs using that k-mer
		for (int i = 0; i < finalMotifs.size(); i++)
		{
			// catch identical values and disregard them
			if (newMotif == finalMotifs[i].first &&
				motifScore == finalMotifs[i].second)
					break;
			// compare k-mer score to best motifs
			if (motifScore > finalMotifs[i].second)
			{
				finalMotifs[i] = make_pair(newMotif, motifScore);
				break;
			}
		}
		// simple counter to see how many loops it has been
		count++;
		// check to see if the final motifs meet minimum probability score
		bool breakOut = true;
		for (int i = 0; i < finalMotifs.size(); i++)
		{
			if (finalMotifs[i].second <= minScore)
				breakOut = false;
		}
		// break out of the loop if it does
		if (breakOut)
			break;
	}
	
	// write the final motifs to a file
	string fileName = "a2_3_abraham-kukkadam.o3";
	ofstream motifFile(fileName, ios_base::app);
	for (int i = 0; i < finalMotifs.size(); i++)
		motifFile << finalMotifs[i].first << "\n\n";
	motifFile << d << endl;
}

int main()
{
	// QUESTION 1-3 IMPLEMENTATION
	dnaGenerator(3, 7, 5, 1, "a2_1_abraham-kukkadam.fasta");
	bruteForceMotif("a2_1_abraham-kukkadam", 5, 1);
	gibbsSampling("a2_1_abraham-kukkadam", 5, 1);

	
	// QUESTION 4 TESTING:
	
	int len[] = { 25, 100, 250 };
	int num[] = { 5, 25, 50 };
	/*
	// PART A
	for (int i = 0; i < 3; i++)
	{
		string file = "Q4_PtA" + to_string(i+1) + ".fasta";
		dnaGenerator(10, len[i], 15, 4, file);

		clock_t time1 = clock();
		bruteForceMotif(file, 15, 4, i+1);
		time1 = clock() - time1;
		clock_t time2 = clock();
		gibbsSampling(file, 15, 4, i+1);
		time2 = clock() - time2;

		cout << " Length " << len[i] << ": \n";
		cout << " ------------------\n";
		cout << " Brute Force Search:" << ((float)time1 / CLOCKS_PER_SEC) << endl;
		cout << " Gibbs Sampling: " << ((float)time2 / CLOCKS_PER_SEC) << endl;
		cout << endl;
	}

	// PART B
	for (int i = 0; i < 3; i++)
	{
		string file = "Q4_PtA" + to_string(i + 1) + ".fasta";
		dnaGenerator(num[i], 25, 15, 4, file);

		clock_t time1 = clock();
		bruteForceMotif(file, 15, 4, i + 1);
		time1 = clock() - time1;
		clock_t time2 = clock();
		gibbsSampling(file, 15, 4, i + 1);
		time2 = clock() - time2;

		cout << " # of Sequences: " << num[i] << ": \n";
		cout << " ------------------\n";
		cout << " Brute Force Search:" << ((float)time1 / CLOCKS_PER_SEC) << endl;
		cout << " Gibbs Sampling: " << ((float)time2 / CLOCKS_PER_SEC) << endl;
		cout << endl;
	}*/
	return 0;
}