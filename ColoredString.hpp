/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ColoredString.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 20:18:37 by plouvel           #+#    #+#             */
/*   Updated: 2022/09/08 22:10:35 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOREDSTRING_CLASS_HPP
# define COLOREDSTRING_CLASS_HPP

# include <string>
# include <list>

namespace ft
{
	/* ColoredString a color-customizable string.
	 * The class inherits from std::string, allowing it to be used as a normal string.
	 * A color must be used using ColoredString::<color>, and style atttributes using ColoredString::Style::<style>. */
	class ColoredString : public std::string
	{
		public:

			typedef std::list<std::string>				attrib_container;
			typedef attrib_container::iterator			attrib_iterator;
			typedef const char*								ANSICode;

			class InvalidColor : public std::exception
			{
				public:
					virtual const char*	what(const char *msg) throw();
			};

			class InvalidAttribute : public std::exception
			{
				public:
					virtual const char*	what(const char *msg) throw();
			};

			ColoredString();
			explicit ColoredString(const char *value);
			explicit ColoredString(const std::string& value);
			ColoredString(ANSICode color, const char *value);
			ColoredString(ANSICode color, const std::string& value);
			ColoredString(const ColoredString& src);
			ColoredString&	operator=(const ColoredString& rhs);
			~ColoredString();

			ColoredString&			addAttribute(ANSICode style);
			ColoredString&			removeAttribute(ANSICode style);

			void					setColor(ANSICode color);
			void					setBackgroundColor(ANSICode color);
			const std::string&		getColor() const;

			void					restore();

			friend std::ostream&	operator<<(std::ostream& out, ColoredString& val);

			struct Style
			{
				static ANSICode		Bold;
				static ANSICode		Italic;
				static ANSICode		Underline;
				static ANSICode		CrossedOut;
				static ANSICode		SlowBlink;
			};

			static ANSICode		Default;
			static ANSICode		Black;
			static ANSICode		Red;
			static ANSICode		Green;
			static ANSICode		Yellow;
			static ANSICode		Blue;
			static ANSICode		Magenta;
			static ANSICode		Cyan;
			static ANSICode		White;

			static ANSICode		BrightBlack;
			static ANSICode		BrightRed;
			static ANSICode		BrightGreen;
			static ANSICode		BrightYellow;
			static ANSICode		BrightBlue;
			static ANSICode		BrightMagenta;
			static ANSICode		BrightCyan;
			static ANSICode		BrightWhite;

		private:
			std::string			_color;
			std::string			_outputCode;
			attrib_container	_textAttributes;
			bool				_needBuild;

			attrib_iterator	_isAttribSet(const std::string& attrib);
			void			_buildOutputCode();
			bool			_isValidColor(ANSICode color);
			bool			_isValidAttribute(ANSICode color);
	};
}

#endif
