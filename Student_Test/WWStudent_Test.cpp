// Works at least up to c++23

#include <iostream>
#include <sstream>
#include <string>


//! Macros

//* Macros for SFINAE check of Members inside of a Class 


#define C_HAS_1_MEMBER( STRUCT_N, MEMBER_N1 ) \
template <typename T, typename = void> \
struct STRUCT_N : std::false_type {}; \
\
template <typename T> \
struct STRUCT_N<T , decltype( void( std::declval<T>( ).MEMBER_N1 ) )> : std::true_type {};

#define C_HAS_2_MEMBERS( STRUCT_N, MEMBER_N1, MEMBER_N2 ) \
template <typename T, typename = void> \
struct STRUCT_N : std::false_type {}; \
\
template <typename T> \
struct STRUCT_N<T , std::void_t<decltype( std::declval<T>( ).MEMBER_N1 ) ,decltype( std::declval<T>( ).MEMBER_N2 )>>: std::true_type {};

#define C_HAS_3_MEMBERS( STRUCT_N, MEMBER_N1, MEMBER_N2, MEMBER_N3 ) \
template <typename T, typename = void> \
struct STRUCT_N : std::false_type {}; \
\
template <typename T> \
struct STRUCT_N<T , std::void_t<decltype( std::declval<T>( ).MEMBER_N1 ), decltype( std::declval<T>( ).MEMBER_N2 ), decltype( std::declval<T>( ).MEMBER_N3 )>>: std::true_type {};


//* Macros for Output of CREATE_HAS_MEMBER


#define C_CHECK_MEMBER( NAME, STRUCT_N ) \
template <typename T> \
bool NAME( ) { \
 if (STRUCT_N<T>::value) { \
        std::cout << "+ Found its Member" << std::endl; \
        true;} else { \
        std::cout << "+ Member not found" << std::endl; \
        false;} } \


#define C_CHECK_MEMBERS( NAME, STRUCT_N ) \
template <typename T> \
bool NAME( ) { \
 if (STRUCT_N<T>::value) { \
        std::cout << "+ Found its Members" << std::endl; \
        true;} else { \
        std::cout << "+ Members not found" << std::endl; \
        false;} } \


//* Macro for Dummy Class


#define C_DUMMY_CLASS( CLASS_N ) \
namespace TASK::TESTER{ using CLASS_N = No; } \


//* Macro for t_Check Call


#define C_CHECK( NAME, CHECK_MEMBER ) \
template <typename T> \
bool NAME( ) { \
    if ( check_class<T>( ) && CHECK_MEMBER<T>( ) ) \
        { return true; } else { return false; }} \

//=// End of Macros 


//! STUDENT Code


namespace STUDENT {

//* Students code starts here . . .


    //#include <iostream>   // Have to comment out cos formatter doing weird formatting 

    using namespace std;

    class MyClassX {
        public:
        void printed( ) { cout << "Printed..." << endl; }
        };
    /*
    class MyClassY {
        public:
        int printed;
        int IDE;
        };
    */
    class MyClassZ {
        public:
        void OHM( ) {};
        double AMP;
        float VOLT;
        };

    int main( ) {
        MyClassX x;
        x.printed( );
        return 0;
        }


//* Student code ends here.

    }

//=// End of STUDENT Code


//! Dummy Classes


namespace TASK::TESTER { struct No {}; }

C_DUMMY_CLASS( MyClassX )

C_DUMMY_CLASS( MyClassY )

C_DUMMY_CLASS( MyClassZ )

//=// End of Dummy Classes


//! SFINAE Templates for Members inside a Class


C_HAS_1_MEMBER( checks_for_ClassX , printed( ) )

C_HAS_2_MEMBERS( checks_for_MyClassY , printed , IDE )

C_HAS_3_MEMBERS( checks_for_MyClassZ , OHM( ) , AMP , VOLT )

//=// End of SFINAE Templates for Members inside a Class


//! STUDENT::main Output-Handling


//? Buffs the cout output of STUDENT::main

template <typename T>
bool check_output( T&& student_main_call , const std::string& expectedOutput ) {
    std::stringstream output_stream;
    std::streambuf* old_buffer = std::cout.rdbuf( output_stream.rdbuf( ) );

    ( student_main_call )( );

    std::cout.rdbuf( old_buffer );

    std::string STUDENTOutput = output_stream.str( );

    if ( STUDENTOutput == expectedOutput ) {
        std::cout << "Student Output matches the expected output." << std::endl;
        std::cout << "Task completed.\n" << std::endl;
        return true;
        }
    else {
        std::cout << "\nStudent Output does not match the expected output."
            << std::endl;
        std::cout << "- Expected Output: " << expectedOutput << std::endl;
        std::cout << "-  Student Output: " << STUDENTOutput << std::endl;
        return false;
        }
    }

//=// End of STUDENT::main Output-Handling


//! Check Members + Output


C_CHECK_MEMBER( check_Member_MyClassX , checks_for_ClassX )

C_CHECK_MEMBERS( check_Members_MyClassY , checks_for_MyClassY )

C_CHECK_MEMBERS( check_Members_MyClassZ , checks_for_MyClassZ )

//=// End of Check Members + Output


//! Class-Handling


template <typename T>
bool check_class( ) {

    if ( !( std::is_same<T , TASK::TESTER::No>::value ) ) {
        std::string strClass = typeid( T ).name( );

        size_t lastIntegerPos = strClass.size( );
        while ( lastIntegerPos > 0 && !std::isdigit( strClass [ lastIntegerPos - 1 ] ) ) {
            lastIntegerPos--;
            }

        std::string strClassReadable { strClass.substr( lastIntegerPos ) };
        strClassReadable.pop_back( );

        std::cout << strClassReadable << " Found" << std::endl;
        return true;
        }
    else {
        std::cout << "Class in Assignment not found. " << std::endl;
        return false;
        }
    }

//=// End of Class-Handling


//! Check Class + Members


C_CHECK( t_check_MyClassX , check_Member_MyClassX )

C_CHECK( t_check_MyClassY , check_Members_MyClassY )

C_CHECK( t_check_MyClassZ , check_Members_MyClassZ )

//=// End of Check Class + Members


//! Main Execution of the Program


//* Final Output


template <typename = void>
void execute2( const std::string& expectedOutput ,
                         const bool& fClass1 , const bool& fClass2 ,
                         const bool& fClass3 , const bool& fClass4 ,
                         const bool& fClass5 ) {
    if ( fClass1 && fClass2 && fClass3 && fClass4 && fClass5 ) {
        if ( check_output( *( STUDENT::main ) , expectedOutput ) ) {
            std::cout << "- - Great Job! - -\n" << std::endl;
            }
        else { std::cout << "Try Again!\n" << std::endl; }
        }
    else {
        std::cout << "You forgot the member inside the class/struct!\n" << std::endl;
        std::cout << "Try Again!\n" << std::endl;
        }
    }


//* Class-Call


namespace STUDENT::TASK {
    using namespace ::TASK::TESTER;
    void execute( ) {

        bool fClass1 = t_check_MyClassX<MyClassX>( );
        bool fClass2 = t_check_MyClassY<MyClassY>( );
        bool fClass3 = t_check_MyClassZ<MyClassZ>( );
        bool fClass4 = true;
        bool fClass5 = true;

        // Always end with "\n"
        std::string expectedOutput = "Printed...\n";

        ::execute2<>( expectedOutput , fClass1 , fClass2 , fClass3 , fClass4 , fClass5 );

        }
    }


//* Real Main


int main( ) {
    STUDENT::TASK::execute( );
    return 0;
    }

//=// End of Main Execution of the Program