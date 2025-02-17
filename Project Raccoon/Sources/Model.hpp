#pragma once

#include <filesystem>
namespace fs = std::filesystem;
#include "Common.hpp"

//A lo mejor meter esto en binaries manager. En la versión 2d queda más bonito

void writeModel(const std::vector<p3>& model) {
	std::string basePath = "Resources/Plain models/";
	std::string modelPath;

	std::cout << "Current models: " << std::endl;
	for (const auto& entry : fs::directory_iterator(basePath)) {
		std::cout << "     " << entry.path().filename() << std::endl;
	}
	std::cout << "Enter model name: ";
	std::cin >> modelPath;
	std::string path = basePath + modelPath;
	std::cout << "Setting model in: " << path << std::endl;

	std::ofstream outFile(path, std::ios::binary);
	if (outFile) {
		size_t size = model.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

		// Write each point in the model
		outFile.write(reinterpret_cast<const char*>(model.data()), size * sizeof(p3));
	}
	else {
		std::cerr << "Error opening file for writing." << std::endl;
	}
	outFile.close();
}



// Function to read a model from a binary file
std::vector<p3> readModel() {
	std::string basePath = "Resources/Plain models/";
	std::string modelPath;
	std::cout << "set model path to one of these: " << std::endl;
	for (const auto& entry : fs::directory_iterator(basePath)) {
		std::cout << "     " << entry.path().filename() << std::endl;
	}
	std::cout << "Enter model name: ";
	std::cin >> modelPath;
	std::vector<p3> model;
	std::string path = basePath + modelPath;

	std::ifstream inFile(path, std::ios::binary);
	if (inFile) {
		size_t size;
		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		model.resize(size);
		inFile.read(reinterpret_cast<char*>(model.data()), size * sizeof(p3));
	}
	else {
		std::cerr << "Error opening file for reading." << std::endl;
	}
	inFile.close();

	return model;
}

std::vector<p3> readModel(const std::string modelPath) {
	std::string basePath = "Resources/Plain models/";

	std::vector<p3> model;
	std::string path = basePath + modelPath;

	std::ifstream inFile(path, std::ios::binary);
	if (inFile) {
		size_t size;
		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		model.resize(size);
		inFile.read(reinterpret_cast<char*>(model.data()), size * sizeof(p3));
	}
	else {
		std::cerr << "Error opening file for reading." << std::endl;
	}
	inFile.close();

	return model;
}

// Function to read ASCII STL file similar to your custom style
void readSTL(std::vector<p3>& positions, std::vector<p3>& normals, const std::string& filepath) {
	std::string basePath = "Resources/stl models/";
	std::ifstream inFile(basePath + filepath);



	if (!inFile) {
		std::cerr << "Unable to open file: " << filepath << std::endl;
		return;
	}

	std::string line;
	p3 normal, vertex;

	while (std::getline(inFile, line)) {
		std::istringstream iss(line);
		std::string word;
		iss >> word;

		if (word == "facet") {
			// Read normal vector
			iss >> word; // Skip "normal"
			iss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);
			//print(normal);
		}
		else if (word == "vertex") {
			// Read and store vertex position
			iss >> vertex.x >> vertex.y >> vertex.z;
			vector<p3> interm;
			interm.push_back(vertex);
			positions.insert(positions.end(), { interm[0].x / 100,interm[0].y / 100,interm[0].z / 100 });
			//print(vertex);
		}
	}

	inFile.close();
}



/// Split a string by a delimiter (e.g. ',') into a list of tokens
std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delimiter)) {
		tokens.push_back(item);
	}
	return tokens;
}

/// Given a LINESTRING-like substring "1.0 2.0, 3.1 4.2, ..."
/// parse out each "lon lat" and convert to p2
std::vector<p2> parseLineStringCoords(const std::string& coordString) {
	std::vector<p2> points;
	// split by comma -> ["1.0 2.0", "3.1 4.2", ...]
	auto commaSeparated = split(coordString, ',');
	for (auto& cs : commaSeparated) {
		std::stringstream ss(cs);
		float lon, lat;
		ss >> lon >> lat;  // read "1.0 2.0" etc.
		points.push_back({ lon, lat });
	}
	return points;
}

void readWKT(std::vector<p2>& positions, const std::string& filepath, vector<vector<p2>>& final)
{
	std::ifstream inFile(filepath);
	if (!inFile) {
		std::cerr << "Unable to open file: " << filepath << std::endl;
		return;
	}
	
	std::string line;
	while (std::getline(inFile, line)) {
		// For simplicity, check if we have LINESTRING or MULTILINESTRING
		// in that whole line (typical for CSV WKT lines).
		// If you have more columns in CSV, you'd parse out just the WKT column.

		if (line.find("LINESTRING") != std::string::npos) {
			// Example:  "...LINESTRING (1.51417 38.6936,1.49 38.70)..."
			// 1) find substring inside parentheses
			size_t start = line.find("(");
			size_t end = line.rfind(")");
			if (start != std::string::npos && end != std::string::npos && end > start) {
				// e.g. "1.51417 38.6936,1.49 38.70"
				std::string coords = line.substr(start + 1, end - (start + 1));

				// 2) parse coords into p2
				auto pts = parseLineStringCoords(coords);
				positions.insert(positions.end(), pts.begin(), pts.end());
				final.push_back(pts);
			}
		}
		else if (line.find("MULTILINESTRING") != std::string::npos) {
			// Example: "...MULTILINESTRING ((x1 y1, x2 y2),(x3 y3, x4 y4))..."
			// We'll have an *outer* parentheses pair, then multiple sets inside.
			size_t start = line.find("((");
			size_t end = line.rfind("))");
			if (start != std::string::npos && end != std::string::npos && end > start) {
				// Inside is something like: "x1 y1, x2 y2),(x3 y3, x4 y4"
				std::string multi = line.substr(start + 2, end - (start + 2));

				// Each sub-linestring is in parentheses, so let's split by ")(" 
				// Easiest is to replace ")(" with some delimiter, e.g. '|', then split.

				// naive approach: replace ")(" with "|"
				// (In a robust parser, you'd do more careful scanning.)
				// We'll do it quickly:
				for (size_t pos = 0; pos + 1 < multi.size(); ) {
					if (multi[pos] == ')' && multi[pos + 1] == '(') {
						multi[pos] = '|';
						multi[pos + 1] = '|';
					}
					pos++;
				}

				// Now we can split on "||"
				auto subStrings = split(multi, '|');
				for (auto& sub : subStrings) {
					// sub might look like: "x1 y1, x2 y2, x3 y3"
					// trim extra parentheses if any:
					// e.g. if it starts with ')' or '('
					// (in practice, it might be already stripped, but let's be safe).
					// We can parse the substring as a single LINESTRING:
					auto pts = parseLineStringCoords(sub);
					positions.insert(positions.end(), pts.begin(), pts.end());
					final.push_back(pts);
				}
			}
		}
		// else if there's "POLYGON" or "MULTIPOLYGON", you can handle similarly if needed
	}

	inFile.close();
}




// Function to write a polyhedra without dividing its surfaces in polygons
void writeSimplePolyhedra(const std::vector<p3>& model, const std::vector<p3>& normals, const std::vector<unsigned int>& indices) {
	std::string basePath = "Resources/Simple polyhedra/";
	std::string modelPath;

	// List available models in the directory
	std::cout << "Current models: " << std::endl;
	for (const auto& entry : fs::directory_iterator(basePath)) 
	{
		std::cout << "     " << entry.path().filename() << std::endl;
	}
	std::cout << "Enter model name: ";
	std::cin >> modelPath;
	std::string path = basePath + modelPath;
	std::cout << "Setting model in: " << path << std::endl;

	std::ofstream outFile(path, std::ios::binary);
	if (outFile) 
	{
		size_t size = model.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(model.data()), size * sizeof(p3));

		size = normals.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(normals.data()), size * sizeof(p3));

		size = indices.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(indices.data()), size * sizeof(unsigned int));
	}
	else 
	{
		std::cerr << "Error opening file for writing." << std::endl;
	}
	outFile.close();
}

void readSimplePolyhedra(std::vector<p3>& model, std::vector<p3>& normals, std::vector<unsigned int>& indices, const std::string& modelPath) {
	std::string basePath = "Resources/Simple polyhedra/";
	std::string path = basePath + modelPath;

	std::ifstream inFile(path, std::ios::binary);
	if (inFile) 
	{
		size_t size;

		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		model.resize(size);
		inFile.read(reinterpret_cast<char*>(model.data()), size * sizeof(p3));

		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		normals.resize(size);
		inFile.read(reinterpret_cast<char*>(normals.data()), size * sizeof(p3));

		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		indices.resize(size);
		inFile.read(reinterpret_cast<char*>(indices.data()), size * sizeof(unsigned int));
	}
	else 
	{
		std::cerr << "Error opening file for reading." << std::endl;
	}
	inFile.close();
}
