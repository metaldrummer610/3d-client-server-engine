#ifndef WAVEFRONT_LOADER_H
#define WAVEFRONT_LOADER_H

#include <list>
#include <boost/shared_ptr.hpp>

#include "model.h"
#include "../abstractModel.h"

class WavefrontParser;

/**
 * Loads the wavefront objects
 */
class WavefrontLoader
{
public:
	/**
	 * Default constructor
	 */
	WavefrontLoader();
	/**
	 * Default deconstructor
	 */
	~WavefrontLoader();

	/**
	 * Loads the wavefront object
	 * from a file.
	 * @param str the filename
	 * @returns The wavefront model as a memory managed
	 * or an invalid memory managed pointer on error.
	 */
	AbstractModel* load(const char* str);
protected:
private:
	boost::shared_ptr<WavefrontParser> parser;
};
#endif//WAVEFRONT_LOADER_H
