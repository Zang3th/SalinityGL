#pragma once

#include <string>
#include <opencv2/opencv.hpp>

class Heightmap
{
private:
	int m_height;
	int m_width;
	int m_channels;
	double m_min_value;
	double m_max_value;
	cv::Mat m_img;
	std::string m_filepath;
	int m_flag;	

public:
	int m_valOutOfBounds;

	Heightmap(std::string filepath, int flag)
		: m_filepath(filepath), m_flag(flag), m_valOutOfBounds(0)
	{
		//Einlesen des Bildes
		m_img = cv::imread(filepath, flag);
		if (!m_img.data)
			spdlog::error("Heightmap failed to load: {}", filepath);

		//Gewinnen der Bilddaten
		m_height = m_img.rows;
		m_width = m_img.cols;
		m_channels = m_img.channels();	
		
		spdlog::info("Heightmap loaded successfully: {}", filepath);

		//Ermittlung von Minimum und Maximum
		minMaxLoc(m_img, &m_min_value, &m_max_value);
	}

	void display() const
	{
		cv::imshow("Heightmap", m_img);
	}

	float getPixelValue(int x, int z, float scale)
	{
		if ((x < m_width) && (z < m_height))
		{
			float greyvalue = m_img.at<uchar>(x, z);
			greyvalue -= m_max_value / 2;
			greyvalue /= scale;
			return greyvalue;
		}
		else
		{
			int width_dif = x - m_width;
			int height_dif = z - m_height;

			if ((width_dif >= 0) && (height_dif >= 0))
			{
				m_valOutOfBounds++;
				float greyvalue = m_img.at<uchar>(x - width_dif - 1, z - height_dif - 1);
				greyvalue -= m_max_value / 2;
				greyvalue /= scale;
				return greyvalue;
			}

			if (width_dif >= 0)
			{
				m_valOutOfBounds++;
				float greyvalue = m_img.at<uchar>(x - width_dif - 1, z);
				greyvalue -= m_max_value / 2;
				greyvalue /= scale;
				return greyvalue;
			}

			if (height_dif >= 0)
			{
				m_valOutOfBounds++;
				float greyvalue = m_img.at<uchar>(x, z - height_dif - 1);
				greyvalue -= m_max_value / 2;
				greyvalue /= scale;
				return greyvalue;
			}
		}
	}

	glm::vec3 getColorValue(int x, int z)
	{
		if ((x < m_width) && (z < m_height))
		{
			float blue = m_img.at<cv::Vec3b>(x, z)[0];
			float green = m_img.at<cv::Vec3b>(x, z)[1];
			float red = m_img.at<cv::Vec3b>(x, z)[2];

			if(blue > green)
			{
				if(blue > red)
				{
					blue = 255;
					green = 0;
					red = 0;
				}
			}

			if(green > blue)
			{
				if(green > red)
				{
					green = 255;
					blue = 0;
					red = 0;
				}
			}

			if(red > blue)
			{
				if(red > green)
				{
					red = 255;
					blue = 0;
					green = 0;
				}
			}

			return glm::vec3(blue, green, red);

		}
		else 
			return glm::vec3(0, 255, 0);
	}
};