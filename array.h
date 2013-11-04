#pragma once
#include <fstream>
using namespace std;

template <class T> class arr{
	T* ar;
public:
	int maxlen;
	int size;
	explicit arr<T>(int maxlength): maxlen(maxlength), size(0){
		ar=new T[maxlen];
	}
	~arr<T>(){
		delete [] ar;
	}
	void add(const T &elem){
		ar[size]=elem;
		++size;
		if (size>maxlen)
			throw "not enough memory";
	}
	const T& operator[](int i) const{
		return ar[i];
	}
	T& operator[](int i){
		if (size<i+1){
			size=i+1;
			if (size>maxlen)
				throw "not enough memory";
		}
		return ar[i];
	}
	explicit arr<T> (const arr<T> &ar1):maxlen(ar1.maxlen), size(ar1.size){
		ar = new T[ar1.maxlen];
		memcpy(ar, ar1.ar, sizeof(T) * ar1.size);		
	}
	arr<T>& operator=(const arr<T> &ar1){
		delete[] ar;
		maxlen = ar1.maxlen;
		ar = new T[ar1.maxlen];
		memcpy(ar, ar1.ar, sizeof(T) * ar1.size);
		size = ar1.size;
		return *this;
	}
	void reset(){
		size=0;
	}
	friend ostream& operator<< <>(ostream &out, const arr<T> &ar); /*{
		ar.ar[ar.size]='\0';
		out << ar.ar;
		return out;
	}*/
};

template <class T> ostream& operator<<(ostream &out, const arr<T> &ar){
	char* str = new char[ar.size + 1];
	memcpy(str, ar.ar, ar.size * sizeof(char));
	str[ar.size]='\0';
	out << str;
	delete[] str;
	return out;
}