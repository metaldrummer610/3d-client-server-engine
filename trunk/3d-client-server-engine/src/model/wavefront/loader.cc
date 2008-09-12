#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <iterator>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
using boost::bind;
using boost::ref;
using boost::function;
using boost::shared_ptr;

#include "loader.h"

using namespace std;

/**
 * Converts the source type to the
 * destination type, using a given
 * default if conversion fails.
 *
 * @param str the source type
 * @param def the default value to use
 * @returns The converted type or the default value
 */
template <typename T,typename SRC>
T convert(SRC str,T def)
{
	try {
		return boost::lexical_cast<T>(str);
	}catch(const boost::bad_lexical_cast&)
	{
		return def;
	}
}

/**
 *	Splits a line into three separate components
 *	and places the components in a list.
 *
 *	@param str the string to split
 *	@param ret the destination list
 */
template <typename T>
void special_split(string& str,T& ret)
{
	int a=0;
	int b=0;
	int c=0;

	if(str.find_first_of("/") != string::npos) //split into 3 parts
	{
		size_t x,y;

		// first section
		x = 0;
		y = str.find_first_of('/');

		a = convert(str.substr(x,y),0);

		// second section, may be ""
		x = str.find_first_of('/')+1;
		y = str.substr(x,str.length()).find_first_of('/');

		if(x == y) {
			b = 0;
		}else{
			b = convert<int>(str.substr(x,y),0);
		}

		// third section
		x = str.find_last_of("/")+1;
		y = str.length();

		c = convert<int>(str.substr(x,y),0);
	}else{ //just one part
		a = convert<int>(str,0);
		b = 0;
		c= 0;
	}

	ret.push_back(a);
	ret.push_back(b);
	ret.push_back(c);
}

/**
 * Utility class for parsing the wavefront file format
 */
class WavefrontParser
{
public:
	friend class WavefrontLoader;
protected:
	WavefrontParser()
	{
		callbacks["v"] = bind(&WavefrontParser::callback_v,ref(*this),_1,_2);
		callbacks["vn"] = bind(&WavefrontParser::callback_vn,ref(*this),_1,_2);
		callbacks["f"] = bind(&WavefrontParser::callback_f,ref(*this),_1,_2);
//		callbacks["texture-image"] = bind(&WavefrontParser::callback_texture_image, ref(*this),_1,_2);
	}

	typedef istringstream& param_t;
	typedef string key_t;
	typedef function<void (param_t,WavefrontModel& model) > value_t;
	typedef map<key_t,value_t> callbacks_t;
	callbacks_t callbacks;

private:
	void callback_f(param_t ss,WavefrontModel& model)
	{
		static list<int> vertices;
		static list<int> normals;
		static list<int> textures;
		static vector<int> params;

		normals.clear();
		vertices.clear();
		textures.clear();

		string f;

		while(!ss.eof())
		{
			params.clear();
			ss >> f;
			stringstream s(f);
			special_split(f,params);

			vertices.push_back(params[0]);
			textures.push_back(params[1]);
			normals.push_back(params[2]);
		}
		model.addFace(	vertices.begin(),vertices.end(),
						textures.begin(),textures.end(),
						normals.begin(),normals.end());
	}

	void callback_v(param_t ss,WavefrontModel& model)
	{
		static vector<float> coords(4);
		coords[3] = 1.0;

		int x;
		for(x = 0; !ss.eof() && x!=3; x++)
		{
			ss >> coords[x];
		}

		if(x < 3)
		{
			cerr << " not enough coords in line" << endl;
		}

		model.addVertex(coords[0],coords[1],coords[2],coords[3]);
	}

	void callback_vn(param_t ss,WavefrontModel& model)
	{
		static vector<float> coords(3);
		int x = 0;
		for(x = 0; !ss.eof() && x!=2;x++)
		{
			ss >> coords[x];
		}

		if(x < 2)
		{
			cerr << " not enough coords in line vn" << endl;
		}
		model.addNormal(coords[0],coords[1],coords[2]);
	}
};

WavefrontLoader::WavefrontLoader()
:parser(new WavefrontParser)
{
}

WavefrontLoader::~WavefrontLoader()
{

}

void rm_white(string &str) {
	str.erase(remove_if(str.begin(), str.end(),
	bind2nd(equal_to<char>(), ' ')), str.end());
}

AbstractModel*
WavefrontLoader::load(const char* str)
{
	WavefrontModel* model = new WavefrontModel();

	model->setName(str);

	WavefrontParser::callbacks_t& callbacks = parser->callbacks;
	istringstream param;

	ifstream file(str);
	string input;
	string value;

	if(file.is_open())
	{
		while(!file.eof())
		{
			getline(file,input);
			param.clear();
			param.str(input);
			value = "";
			param >> value;
			try {
				callbacks[value](param,*model);
			}catch(boost::bad_function_call& b)
			{
				//cerr << "invalid wavefront spec (" << value << ")" <<  endl;
			}
		}
	}



	return (AbstractModel*)model;
}

