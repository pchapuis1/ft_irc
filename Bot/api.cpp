/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfinette <mfinette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:51:20 by mfinette          #+#    #+#             */
/*   Updated: 2024/03/26 11:31:20 by mfinette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

#define EDEN_API_KEY getApiKey()
#define EDEN_API_IMAGE_ENDPOINT "https://api.edenai.run/v2/image/generation"
#define EDEN_API_CHATBOT_ENDPOINT "https://api.edenai.run/v2/text/chat"



std::string extract_image_url(const std::string& data)
{
	std::string url;
	std::string search_str = "\"image_resource_url\":\"";
	size_t start_index = data.find(search_str);
	if (start_index == std::string::npos)
		return url;  // If the substring is not found, return an empty string
	start_index += search_str.length();
	size_t end_index = data.find("\"", start_index);
	if (end_index == std::string::npos)
		return url;  // If the closing quote is not found, return an empty string
	url = data.substr(start_index, end_index - start_index);
	return url;
}

std::string extractText(const std::string& data)
{
	std::string url;
	std::string search_str = "\"generated_text\":\"";
	size_t start_index = data.find(search_str);
	if (start_index == std::string::npos)
		return url;  // If the substring is not found, return an empty string
	start_index += search_str.length();
	size_t end_index = data.find("\"", start_index);
	if (end_index == std::string::npos)
		return url;  // If the closing quote is not found, return an empty string
	url = data.substr(start_index, end_index - start_index);
	return url;
}

std::string	extractCost(const std::string& data)
{
	std::string url;
	std::cout << "extracting cost" << std::endl;
	std::string search_str = "\"cost\":";
	size_t start_index = data.find(search_str);
	if (start_index == std::string::npos)
		return url;  // If the substring is not found, return an empty string
	start_index += search_str.length();
	size_t end_index = data.find("}", start_index);
	if (end_index == std::string::npos)
		return url;  // If the closing quote is not found, return an empty string
	url = data.substr(start_index, end_index - start_index);
	return url;

}

std::string getImageFromAPI(const char *prompt)
{
	std::string providers = "amazon";
	std::string model = "titan-image-generator-v1_premium";
	std::string resolution = "1024x1024";
	std::string jsonData = "{\"response_as_dict\":true,\"attributes_as_list\":false,\"show_original_response\":false,\"resolution\":\"" + resolution + "\",\"num_images\":1,\"providers\":\"" + providers + "\",\"text\":\"" + std::string(prompt) + "\",\"model\":\"" + model + "\"}";
	std::string command = "curl -s -X POST -H \"accept: application/json\" -H \"content-type: application/json\" -H \"Authorization: Bearer " + std::string(EDEN_API_KEY) + "\" -d '" + jsonData + "' " + std::string(EDEN_API_IMAGE_ENDPOINT);
	FILE* pipe = popen(command.c_str(), "r");
	if (!pipe)
		return "Error when using popen function";
	char buffer[256];
	std::string response;
	while (fgets(buffer, sizeof(buffer), pipe) != NULL)
		response += buffer;
	pclose(pipe);
	std::cout << response << std::endl;
	if (response.find("error") != std::string::npos)
		return "Error when using the API";
	if (response.find("malformated,") != std::string::npos)
		return "Error when using the API";
	return response;
}

std::string getChatAnswerFromAPI(const char* prompt)
{
	std::string providers = "openai";
	std::string model = "gpt-4-vision-preview";
	std::string global_action = std::string(BOT_GLOBAL_INFO);
	std::string jsonData = "{\"response_as_dict\":true,\"attributes_as_list\":false,\"show_original_response\":false,\"temperature\":0,\"max_tokens\":1000,\"chatbot_global_action\":\"" + global_action + "\",\"text\":\"" + std::string(prompt) + "\",\"providers\":\"" + providers + "\"}";
	std::string command = "curl -s -X POST -H \"accept: application/json\" -H \"content-type: application/json\" -H \"Authorization: Bearer " + std::string(EDEN_API_KEY) + "\" -d '" + jsonData + "' " + std::string(EDEN_API_CHATBOT_ENDPOINT);
	FILE* pipe = popen(command.c_str(), "r");
	if (!pipe)
		return "Error when using popen function";
	char buffer[256];
	std::string response;
	while (fgets(buffer, sizeof(buffer), pipe) != NULL)
		response += buffer;
	if (response.find("malformated") != std::string::npos)
		return "Error when using the API";
	std::cout << "response: " << response << std::endl;
	pclose(pipe);
	return response;
}
