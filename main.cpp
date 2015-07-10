#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdint>
#include <boost/math/special_functions/prime.hpp>

static const std::size_t kNbItems( 1000000 );

namespace method1
{

void removeDuplicates( std::vector<uint32_t> & items )
{
    using namespace boost::math;
    uint64_t checker = 1u; // Be careful of integer overflow here

    std::size_t n = 0;
    for( const uint32_t x : items )
    {
        const uint32_t p = prime( x );
        if ( checker % p )
        {
            checker *= p;
            items[n++] = x;
        }
    }

    items.resize( n );
}

}

namespace method2
{

void removeDuplicates( std::vector<uint32_t> & items )
{
    std::sort( items.begin(), items.end() );
    items.erase( std::unique( items.begin(), items.end() ), items.end() );
}

}

/**
 * @brief main goes here
 */
int main( int argc, char **argv )
{
    std::mt19937 rng;
    rng.seed( time( 0 ) );

    std::uniform_int_distribution<uint32_t> rand( 0, 10 );
    std::vector<uint32_t> v;
    v.reserve( kNbItems );
    for( std::size_t i = 0; i < kNbItems; ++i )
    {
        v.push_back( rand( rng ) );
    }

    // Benchmark
    typedef std::chrono::high_resolution_clock Clock;
    using std::chrono::microseconds;
    using std::chrono::duration_cast;

    std::vector<uint32_t> uniqueItems1( v );
    {
        auto t1 = Clock::now();
        method1::removeDuplicates( uniqueItems1 );
        auto t2 = Clock::now();
        std::cout << "Method1 took : " << duration_cast<microseconds>(t2-t1).count() << std::endl;
    }

    std::vector<uint32_t> uniqueItems2( v );
    {
        auto t1 = Clock::now();
        method2::removeDuplicates( uniqueItems2 );
        auto t2 = Clock::now();
        std::cout << "Method2 took : " << duration_cast<microseconds>(t2-t1).count() << std::endl;
    }
    
    std::sort( uniqueItems1.begin(), uniqueItems1.end() );
    std::sort( uniqueItems2.begin(), uniqueItems2.end() );
    if ( uniqueItems1 == uniqueItems2 )
    {
        std::cout << "RESULTS ARE CONFORM" << std::endl;
    }
    else
    {
        std::cerr << "OUPS... Algorithm outputs different resutls!" << std::endl;
    }
    std::cout << "uniqueItems1.size(): " << uniqueItems1.size() << std::endl;
    std::cout << "uniqueItems2.size(): " << uniqueItems2.size() << std::endl;
}
