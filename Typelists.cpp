#include <iostream>
#include <type_traits>
using namespace std;

struct NullType{};

template<typename T, typename U>
struct ListNode{
	typedef T head;
	typedef U tail;
};

#define LIST_1(T1) ListNode<T1,NullType>
#define LIST_2(T1,T2) ListNode<T1,LIST_1(T2)>
#define LIST_3(T1,T2,T3) ListNode<T1,LIST_2(T2,T3)>

template <typename ListNode> struct ListLength;
template<typename T, typename U>
struct ListLength<ListNode<T,U>> {
	static const int value = 1 + ListLength<U>::value;
};

template<>
struct ListLength<NullType> {
	static const int value = 0;	
};

template <typename ListNode, unsigned int index> struct TypeAt;
template<typename T, typename U, unsigned int index>
struct TypeAt<ListNode<T,U>, index>
{
	typedef typename TypeAt<U,index-1>::type type;
};

template<typename T, typename U>
struct TypeAt<ListNode<T,U>,0> {
	typedef T type;
};

template <typename ListNode, typename S> struct IndexOf;
template <typename S>
struct IndexOf<NullType, S>
{
	static const int value = -1;	
};

template <typename T, typename U>
struct IndexOf<ListNode<T,U>,T>
{
	static const int value = 0;	
};

template <typename T, typename U, typename S>
struct IndexOf<ListNode<T,U>,S>
{
	static const int value = (IndexOf<U,S>::value == -1) ? (-1) : (1 + IndexOf<U,S>::value); 
};


template<typename ListNode, typename A> struct AppendTo;
template<typename T, typename U, typename A>
struct AppendTo<ListNode<T,U>,A> {
	typedef ListNode<T,typename AppendTo<U,A>::type> type;
};

template<typename A>
struct AppendTo<NullType, A> {
	typedef LIST_1(A) type;
};

template<typename T, typename U>
struct AppendTo<NullType, ListNode<T,U>> {
	typedef ListNode<T,U> type;
};


int main() {
	typedef LIST_3(int, long, float) Some3Types;
	static_assert(ListLength<Some3Types>::value == 3);
	static_assert(is_same<TypeAt<Some3Types,0>::type,int>::value);
	static_assert(IndexOf<Some3Types,long>::value == 1);
	typedef AppendTo<Some3Types,double>::type Some4Types;
	static_assert(ListLength<Some4Types>::value == 4);
	static_assert(IndexOf<Some4Types,double>::value == 3);
	return 0;
}
