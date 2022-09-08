/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ColoredString.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 13:49:49 by plouvel           #+#    #+#             */
/*   Updated: 2022/09/08 22:10:38 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ColoredString.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

#include "ansi_color_codes.h"

namespace ft
{
	// Constant declaration
	ColoredString::ANSICode ColoredString::Style::Bold = "1";
	ColoredString::ANSICode ColoredString::Style::Italic = "3";
	ColoredString::ANSICode ColoredString::Style::Underline = "4";
	ColoredString::ANSICode ColoredString::Style::CrossedOut = "9";
	ColoredString::ANSICode ColoredString::Style::SlowBlink = "5";

	ColoredString::ANSICode ColoredString::Black = "30";
	ColoredString::ANSICode ColoredString::Red = "31";
	ColoredString::ANSICode ColoredString::Green = "32";
	ColoredString::ANSICode ColoredString::Yellow = "33";
	ColoredString::ANSICode ColoredString::Blue = "34";
	ColoredString::ANSICode ColoredString::Magenta = "35";
	ColoredString::ANSICode ColoredString::Cyan = "36";
	ColoredString::ANSICode ColoredString::White = "37";

	ColoredString::ANSICode ColoredString::BrightBlack = "90";
	ColoredString::ANSICode ColoredString::BrightRed = "91";
	ColoredString::ANSICode ColoredString::BrightGreen = "92";
	ColoredString::ANSICode ColoredString::BrightYellow = "93";
	ColoredString::ANSICode ColoredString::BrightBlue = "94";
	ColoredString::ANSICode ColoredString::BrightMagenta = "95";
	ColoredString::ANSICode ColoredString::BrightCyan = "96";
	ColoredString::ANSICode ColoredString::BrightWhite = "97";

	ColoredString::ANSICode ColoredString::Default = "39";

	// Default constructor
	ColoredString::ColoredString()
		: _color(ColoredString::Default), std::string(), _needBuild(true), _outputCode("\e[")
	{}

	ColoredString::ColoredString(const char* value)
		: _color(ColoredString::Default), std::string(value), _needBuild(true), _outputCode("\e[")
	{}

	ColoredString::ColoredString(const std::string& value)
		: _color(ColoredString::Default), std::string(value), _needBuild(true), _outputCode("\e[")
	{}

	ColoredString::ColoredString(ANSICode color, const char *value)
		: _color(color), std::string(value), _needBuild(true), _outputCode("\e[")
	{
		if (!this->_isValidColor(color))
			throw (ColoredString::InvalidColor());
	}

	ColoredString::ColoredString(ANSICode color, const std::string& value)
		: _color(color), std::string(value), _needBuild(true), _outputCode("\e[")
	{
		if (!this->_isValidColor(color))
			throw (ColoredString::InvalidColor());
	}

	ColoredString::ColoredString(const ColoredString& src)
		: _color(src._color), std::string(src), _needBuild(src._needBuild), _outputCode(src._outputCode)
	{}

	ColoredString&	ColoredString::operator=(const ColoredString& rhs)
	{
		if (rhs != *this)
		{
			_color = rhs._color;
			_outputCode = rhs._outputCode;
			_textAttributes = rhs._textAttributes;
			_needBuild = rhs._needBuild;
		}
		return (*this);
	}

	ColoredString::~ColoredString()
	{}

	/* Set an attribute for the current string.
	 * Throws an exception if the attribute is invalid. */
	ColoredString& ColoredString::addAttribute(ANSICode style)
	{
		if (!this->_isValidAttribute(style))
			throw (ColoredString::InvalidAttribute());
		if (this->_isAttribSet(style) == _textAttributes.end())
		{
			_textAttributes.push_back(style);
			_needBuild = true;
		}
		return(*this);
	}

	// Remove an attribute for the current string.
	ColoredString& ColoredString::removeAttribute(ANSICode style)
	{
		ColoredString::attrib_iterator it;

		it = _isAttribSet(style);
		if (it != _textAttributes.end())
		{
			_textAttributes.erase(it);
			_needBuild = true;
		}
		return (*this);
	}

	/* Set the color for the current string.
	 * Throws an exception if the color is invalid. */
	void	ColoredString::setColor(ANSICode color)
	{
		if (!this->_isValidColor(color))
			throw (ColoredString::InvalidColor());
		_color = color;
	}

	/* Set the background for the current string.
	 * Throws an exception if the color is invalid. */
	void	ColoredString::setBackgroundColor(ANSICode color)
	{
		std::string			strColor(color);
		std::string			newStrColor;
		std::stringstream	ss, valueToStr;
		unsigned int		value;
		
		if (!this->_isValidColor(color))
			throw (ColoredString::InvalidColor());
		ss << strColor;
		ss >> value;
		value += 10;
		valueToStr << value;
		newStrColor = valueToStr.str();
		if (this->_isAttribSet(newStrColor) == _textAttributes.end())
		{
			_textAttributes.push_back(newStrColor);
			_needBuild = true;
		}
	}

	// Return the current color
	const std::string&	ColoredString::getColor() const
	{
		return (_color);
	}

	// Restore the terminal default color, and all attributes are removed.
	void	ColoredString::restore()
	{
		_textAttributes.erase(_textAttributes.begin(), _textAttributes.end());
		_color = ColoredString::Default;
		_needBuild = true;
	}

	std::ostream&	operator<<(std::ostream& out, ColoredString& val)
	{
		if (val._needBuild)
			val._buildOutputCode();
		out << val._outputCode << val.data() << CRESET;
		return (out);
	}

	const char*	ColoredString::InvalidColor::what(const char *msg) throw()
	{
		return ("invalid color");
	}

	const char*	ColoredString::InvalidAttribute::what(const char *msg) throw()
	{
		return ("invalid attribute");
	}

	/* ########## Private member functions ########## */

	ColoredString::attrib_iterator	ColoredString::_isAttribSet(const std::string& attrib)
	{
		return (std::find(_textAttributes.begin(), _textAttributes.end(), attrib));
	}

	void	ColoredString::_buildOutputCode()
	{
		_outputCode.erase(_outputCode.begin() + 2, _outputCode.end()); 
		_outputCode.append(_color);
		if (!_textAttributes.empty())
		{
			for (ColoredString::attrib_iterator it = _textAttributes.begin(); it != _textAttributes.end(); it++)
				_outputCode.append(";").append(*it);
		}
		_outputCode.push_back('m');
		_needBuild = false;
	}

	bool	ColoredString::_isValidColor(ANSICode color)
	{
		std::string			strColor(color);
		std::stringstream	ss;
		unsigned int		value;

		if (strColor.find_first_not_of("0123456789") != std::string::npos)
			return (false);
		ss << strColor;
		ss >> value;
		return ((value >= 30 && value <= 37) || (value >= 90 && value <= 97));
	}

	bool	ColoredString::_isValidAttribute(ANSICode attribute)
	{
		std::string	strAttribute(attribute);
		std::stringstream	ss;
		unsigned int		value;

		if (strAttribute.find_first_not_of("0123456789") != std::string::npos)
			return (false);
		ss << strAttribute;
		ss >> value;
		if (value > 107)
			return (false);
		return (!(value >= 30 && value <= 37) || (value >= 90 && value <= 97));
	}
}
