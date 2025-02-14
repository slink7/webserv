<<<<<<< HEAD
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FT.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 10:05:59 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/13 16:04:18 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

>>>>>>> parsing
#ifndef FT_HPP
#define FT_HPP

#include <string>
#include <cstring>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cerrno>
#include <sys/stat.h>
<<<<<<< HEAD

#include "Log.hpp"

namespace FT {
=======
#include <cstdlib>

#include "Log.hpp"

namespace FT
{
>>>>>>> parsing
	const int RECV_SIZE = 17;

	int	send(int fd, std::string& msg, int len);
	int	send(int fd, const char *msg, int len);
	int	send(int fd, std::string& msg);
	int	send(int fd, const char *msg);

	int	receive(int fd, std::string& out);

	int	read(int fd, std::string& out);

	std::string itoa(int n);

	bool	is_directory(const std::string& path);
	bool	is_file(const std::string& path);

	const std::string& get_mime_type(const std::string& file);

	void	replace(std::string& str, const char *from, const char *to);

<<<<<<< HEAD
	void	trim(std::string& str, const std::string& set);

=======
	std::string	get_value(const std::string& source, const std::string& name);
	int			get_int(const std::string& source, const std::string& name);

	bool		get_file(std::string& dst, const std::string& path);

	int			count(const std::string& src, const std::string& word);
>>>>>>> parsing
}

#endif