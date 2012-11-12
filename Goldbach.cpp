#include <iostream>
#include <vector>
#define ALLOC_ERROR 1

bool get_bounds(int&a, int& b) {
	std::cout << "This program lists all sums of primes with value c, while c is the smallest of the even numbers between a and b that have the fewest of these sums.\n"
	          << "Enter an integer a > 2 (lower boundary): ";
	std::cin >> a;
	int errorcount = 0;
	while ((a <= 2) && (errorcount<=3)) {
		errorcount++;
		std::cout << "This input is not allowed. Try again. \n"
		          << "Enter an integer a > 2 (lower boundary): ";
		std::cin >> a;
	}
	if(errorcount==4) return false;
	errorcount = 0;
	std::cout << "Enter an integer b > a (upper boundary) : ";
	std::cin >> b;
	while ((b <= a) && (errorcount<=3)) {
		errorcount++;
		std::cout << "This input is not allowed. Try again. \n"
		          << "Enter an integer b > a (upper boundary): ";
		std::cin >> b;
	}
	if(errorcount==4) return false;
	return true;
}

int find_primes(std::vector<bool>& p, int a, int b) {
	if(p.capacity()<b+1) return ALLOC_ERROR;
	for (int i = 2; i <= b; ++i) {
		if (p[i]) {
			for (int j = i; j <= b/i; ++j) {
				p[i*j] = false;
			}
		}
	}
	p[0] = p[1] = false;
	return 0;	
}

void goldbach(int a, int b, std::vector<bool>& p) {
	// Es wird für alle geraden c, a <= c <= b, untersucht, wie viele Primzahl-Summen existieren.
	// In c_min wird das kleinste c gespeichert, für das es die wenigsten gibt.
	int sums = 0;
	std::vector<int> locprime_pairs;
	locprime_pairs.reserve(b+1);
	std::vector<int> minprime_pairs;
	minprime_pairs.reserve(b+1);
	int c_min = a;
	int sums_min = b;
	if (a%2) ++a;
	for (int c = a; c <= b; c+= 2) {
		locprime_pairs.clear();
		for (int i = 2; i <= c/2; ++i) {
			if (p[i] && p[c-i]) {
				locprime_pairs.push_back(i);
				locprime_pairs.push_back(c-i);
				++sums;
				if(sums>=sums_min) break; // at this point we know that we can't get a better solution. stop useless calculation
			}
		}
		if (sums < sums_min) {
			c_min = c;
			sums_min = sums;
			minprime_pairs=locprime_pairs; //store current best pairs
		}
		sums = 0;
	}
	
	// Dieser Fall sollte für den überprüfbaren Zahlenbereich nicht eintreffen.
	if (sums_min==0) {
		std::cout << "Congratulations! You found a couterexample for Goldbach's Conjecture.\n"
		          << "There is no sum of two primes for " << c_min << ".\n";
	}
	else {
		// Der gesuchte Wert wird mitsamt den möglichen Summen ausgegeben.
		std::cout << "The sought integer is " << c_min << ".\n"
				  << "There are " << sums_min << " such sums.\n"
				  << "Those are:\n";
		for(int i=0; i<sums_min; i++) {
			std::cout << c_min << " = " << minprime_pairs[2*i] << " + " << minprime_pairs[2*i+1] << "\n";
		}
	}
}

int main() {
	// Dieser Teil des Programms initialisiert den Bereich, für den die Summen untersucht werden sollen.
	int a,b = 0;
	if(!get_bounds(a,b)) {
		std::cout << "Due to repeated wrong inputs this program will now exit, please try again\n";
		return 1;
	}
	// p (ein Vektor) wir so initialisiert, dass gilt: p[i] == true <=> i prim.
	std::vector<bool> p(b+1, true);
	if(find_primes(p,a,b) == ALLOC_ERROR) {
		std::cout << "Problems with memory allocation. Your OS feels sorry\n";	 
		return 1;
	}
	goldbach(a,b,p);	
	return 0;
}

