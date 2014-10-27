#ifndef VectorIO_h
#define VectorIO_h
#include <vector>

template<class T> ostream& operator<<(ostream& ostr, const vector<T>& vec){
	vector<T>::size_type nelem = vec.size();
	ostr.write( &nelem, sizeof( nelem ) );
	vector<T>::const_iterator item = vec.begin();
	while( item != vec.end() ) (item++)->write(ostr);
	return ostr;
}

template<class T> istream& operator>>(istream& istr, vector<T>& vec){
	vec.clear();
	vector<T>::size_type nelem = 0;
	istr.read( &nelem, sizeof( nelem ) );
	while( nelem-- ) vec.push_back( T(istr) );
}

template<class T> istream& operator>>(istream& istr, const vector<T>& v){
	vector<T> *vec = const_cast< vector<T>* >(v);
	vec.clear();
	vector<T>::size_type nelem = 0;
	istr.read( &nelem, sizeof( nelem ) );
	while( nelem-- ) vec.push_back( T(istr) );
}

template<class T> ostream& operator<<(ostream& ostr, const vector<T>*const &vec){
	vector<T>::size_type nelem = vec->size();
	ostr.write( &nelem, sizeof( nelem ) );
	vector<T>::const_iterator item = vec->begin();
	while( item != vec->end() ) (item++)->write(ostr);
	return ostr;
}

template<class T> istream& operator>>(istream& istr, vector<T>*const &vec){
	vec->clear();
	vector<T>::size_type nelem = 0;
	istr.read( &nelem, sizeof( nelem ) );
	while( nelem-- ) vec->push_back( T(istr) );
}

template<class T> istream& operator>>(istream& istr, const vector<T>*const &v){
	vector<T> *vec = const_cast< vector<T>* >(v);
	vec->clear();
	vector<T>::size_type nelem = 0;
	istr.read( &nelem, sizeof( nelem ) );
	while( nelem-- ) vec->push_back( T(istr) );
}

template<class T> ostream& operator<<(ostream& ostr, vector<T>*const &vec){
	vector<T>::size_type nelem = vec->size();
	ostr.write( &nelem, sizeof( nelem ) );
	vector<T>::const_iterator item = vec->begin();
	while( item != vec->end() ) (item++)->write(ostr);
	return ostr;
}

template<class T> istream& operator>>(istream& istr, vector<T>*&vec){
	vec->clear();
	vector<T>::size_type nelem = 0;
	istr.read( &nelem, sizeof( nelem ) );
	while( nelem-- ) vec->push_back( T(istr) );
}

template<class T> istream& operator>>(istream& istr, const vector<T>*&v){
	vector<T> *vec = const_cast< vector<T>* >(v);
	vec->clear();
	vector<T>::size_type nelem = 0;
	istr.read( &nelem, sizeof( nelem ) );
	while( nelem-- ) vec->push_back( T(istr) );
}

#endif
