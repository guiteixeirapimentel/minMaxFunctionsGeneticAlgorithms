#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>

#include "Candidate.hpp"

constexpr auto numberOfVariablesN = 10;
constexpr auto maxGenerations = 5000;
constexpr auto populationSize = 500;

constexpr auto maxValue = 5.12f;
constexpr auto minValue = -5.12f;
constexpr auto tolerance = 1e-5;

constexpr auto showPartialMaxFitting = false;

typedef float real;

template <int N, typename T = float>
auto sphereFunction(const std::array<T, N>& values)
{
	T result = 0.0f;
	for (const auto x : values)
	{
		result += x*x;
	}

	return result;
}

template <int N, typename T=float>
auto rastriginFunction(const std::array<T, N>& values)
{
	T result = 10.0f * N;

	for (const auto x : values)
	{
		result += (x*x) - static_cast<T>(10.0*cos(2.0*M_PI*x));
	}

	return result;
}

const auto function = sphereFunction<numberOfVariablesN, real>;

int main()
{
	Candidate<numberOfVariablesN, real>::setMaxValue(maxValue);
	Candidate<numberOfVariablesN, real>::setMinValue(minValue);

	std::vector<Candidate<numberOfVariablesN, real>> population;
	population.resize(populationSize);
	auto newPopulation = population;

	const auto showFitting = [](const auto maxFitting, const auto currGen, const auto& population) -> void
	{
		std::cout << "---------------------------------" << std::endl;
		std::cout << "Current Generation: " << currGen << std::endl;
		std::cout << "Max fitting: " << maxFitting << std::endl;
		std::cout << "Min Value: " << function(population.front().getValues()) << std::endl;
		std::cout << "---------------------------------" << std::endl;
	};

	const auto sortDescByFitting = [](auto &iterable) -> void
	{
		std::sort(iterable.begin(), iterable.end(), [](const auto &p1, const auto &p2) -> bool
			{ return p1.getFitting(function) > p2.getFitting(function); });
	};
	
	sortDescByFitting(population);

	auto generation = 0;
	for (; generation != maxGenerations; generation++)
	{
		newPopulation[0] = population[0];
		newPopulation[0 + 1] = population[0 + 1];

		for (auto pair = 2; pair < populationSize / 2; pair += 2)
		{
			newPopulation[pair] = population[pair].crossWith(population[pair + 1]);
			newPopulation[pair + 1] = population[pair + 1].crossWith(population[pair]);
		}

		for (auto index = (populationSize / 2) - 1; index < populationSize; index++)
		{
			const auto mateIndex = rand() % populationSize;
			newPopulation[index] = population[index].crossWith(population[mateIndex]);
			newPopulation[index].mutate();
		}

		population = newPopulation;
		sortDescByFitting(population);

		const auto maxFitting = population.front().getFitting(function);

		if (std::abs(maxFitting - 1.0) <= tolerance)
			break;

		if (showPartialMaxFitting)
		{
			showFitting(maxFitting, generation, population);
		}		
	}

	if (!showPartialMaxFitting)
	{
		const auto maxFitting = population.front().getFitting(function);

		showFitting(maxFitting, generation, population);
	}	

	return 0;
}