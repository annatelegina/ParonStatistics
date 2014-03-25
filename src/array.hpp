#pragma once

#include <fstream>
#include <ostream>
#include <cstring>

template <class T>
class array{
	T* ar;
public:
	int maxlen;
	int size;
  
  explicit array<T>(int maxlength): maxlen(maxlength), size(0) {
		ar = new T[maxlen];
	}
  
	~array<T>(){
		delete [] ar;
	}
  
	void add(const T &elem){
		ar[size] = elem;
		++size;
		if (size > maxlen)
			throw "not enough memory";
	}
  
	const T& operator[](int i) const{
		return ar[i];
	}
  
	T& operator[](int i){
		if (size < i+1){
			size = i+1;
			if (size > maxlen)
				throw "not enough memory";
		}
		return ar[i];
	}
  
	array<T>(const array<T> &ar1) : maxlen(ar1.maxlen), size(ar1.size) {
		ar = new T[ar1.maxlen];
		memcpy(ar, ar1.ar, sizeof(T) * ar1.size);		
	}
  
	array<T>& operator=(const array<T> &ar1) {
		delete[] ar;
		maxlen = ar1.maxlen;
		ar = new T[ar1.maxlen];
		memcpy(ar, ar1.ar, sizeof(T) * ar1.size);
		size = ar1.size;
		return *this;
	}
  
	void reset(){
		size = 0;
	}

	std::string toString() const {
		char* str = new char[this->size + 1];
		std::string s;
		memcpy(str, this->ar, this->size * sizeof(char));
		str[this->size] = '\0';
		s = std::string(str);
		delete[] str;
		return s;
	}
  
  template <class oT>
  friend std::ostream& operator<<(std::ostream& out, const array<oT>& ar); 
};

template <class T> 
std::ostream& operator<<(std::ostream& out, const array<T>& ar){
	char* str = new char[ar.size + 1];
	memcpy(str, ar.ar, ar.size * sizeof(char));
	str[ar.size] = '\0';
	out << str;
	delete[] str;
	return out;
}
