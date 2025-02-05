/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 10:05:33 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/04 16:59:54 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Message.hpp"
#include "Method.hpp"
#include "FT.hpp"

namespace HTTP {
	class Request : public Message {

	public:
		Request();
		Request(const std::string& str);

		void		LoadFromString(const std::string& str);
		Method		GetMethod() const;
		const std::string&	GetTarget() const;

	private:
		Method		method;
		std::string	target;
	};
}

#endif