#pragma once

#include <cstdint>
#include <string>
#include <opencv2/opencv.hpp>
#include <iostream>

class Heightmap
{
private:
	int m_height;
	int m_width;
	int m_channels;
	double m_min_value;
	double m_max_value;
	cv::Scalar m_mean;
	cv::Scalar m_stnd_devi;
	cv::Mat m_img;
	std::string m_filepath;
	int m_flag;
	int m_valOutOfBounds;

public:
	Heightmap(std::string filepath, int flag)
		: m_filepath(filepath), m_flag(flag), m_valOutOfBounds(0)
	{
		//Einlesen des Bildes
		m_img = cv::imread(filepath, flag);
		if (!m_img.data) {
			std::cout << "Picture could not be found!\n";
		}

		//Gewinnen der Bilddaten
		m_height = m_img.rows;
		m_width = m_img.cols;
		m_channels = m_img.channels();
		/*std::cout << "Picturesize: " << m_height << "x" << m_width << " with " << m_channels << " Channels" << std::endl;
		std::cout << "Pixels: " << m_height * m_width << std::endl;*/

		//Ermittlung von mittlerem Grauwert, Standardabweichung
		/*meanStdDev(m_img, m_mean, m_stnd_devi);
		std::cout << "Medium Greyvalue: " << m_mean.val[0] << std::endl;
		std::cout << "Standard Deviation: " << m_stnd_devi.val[0] << std::endl;*/

		//Ermittlung von Minimum und Maximum
		minMaxLoc(m_img, &m_min_value, &m_max_value);
		/*std::cout << "Lowest Greyvalue: " << m_min_value << std::endl;
		std::cout << "Highest Greyvalue: " << m_max_value << std::endl << std::endl;*/
	}

	void display() const
	{
		cv::imshow("Heightmap", m_img);
	}

	float getPixelValue(int x, int z, float scale)
	{
		if ((x < m_width) && (z < m_height))
		{
			float greyvalue = m_img.at<uint8_t>(x, z);
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
				float greyvalue = m_img.at<uint8_t>(x - width_dif - 1, z - height_dif - 1);
				greyvalue -= m_max_value / 2;
				//greyvalue /= scale;
				return greyvalue;
			}

			if (width_dif >= 0)
			{
				m_valOutOfBounds++;
				float greyvalue = m_img.at<uint8_t>(x - width_dif - 1, z);
				greyvalue -= m_max_value / 2;
				//greyvalue /= scale;
				return greyvalue;
			}

			if (height_dif >= 0)
			{
				m_valOutOfBounds++;
				float greyvalue = m_img.at<uint8_t>(x, z - height_dif - 1);
				greyvalue -= m_max_value / 2;
				//greyvalue /= scale;
				return greyvalue;
			}
		}
	}

	inline cv::Mat getObject() const { return m_img; }
	inline double getMinValue() const { return m_min_value; }
	inline double getMaxValue() const { return m_max_value; }
	inline int getHeight() const { return m_height; }
	inline int getWidth() const { return m_width; }
	inline int getBadValues() const { return m_valOutOfBounds; }
};