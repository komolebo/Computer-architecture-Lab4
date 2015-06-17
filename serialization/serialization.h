#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/asio.hpp>
#include <string>

using namespace std;

struct _data{
    string Full;
    string Sub;
};


const int myCount( const string & str, const string & obj );
const string serialize(const string &Full, const string &Sub);
const _data deserialize(const boost::array<char, 10000> &buf);


const int myCount( const string & str, const string & obj ) {
    int n = 0;
    std::string ::size_type pos = 0;
    while( (pos = obj.find( str, pos )) != std::string::npos ) {
        n++;
        pos += str.size();
    }
    return n;
}

const string serialize(const string & Full, const string & Sub) {
    ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << Full;
    archive << Sub;

    return archive_stream.str();
}

const _data deserialize(const string message) {
    string archive_data(&message[0], message.size());
    istringstream archive_stream(archive_data);
    boost::archive::text_iarchive archive(archive_stream);

    _data Data;
    archive >> Data.Full;
    archive >> Data.Sub;
    return Data;
}

