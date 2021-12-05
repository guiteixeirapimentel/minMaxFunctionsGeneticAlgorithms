#pragma once
#include <random>
#include <array>
#include <functional>

template <int N, typename T = float>
class Candidate
{
public:
	Candidate()
	{
		for (auto& value : cValues)
		{
			value = getRandomBetween0And1() * (cMaxValue - cMinValue) + cMinValue;
		}
	}
	Candidate(const std::array<T, N>& values)
		:
		cValues(values)
	{}
	~Candidate() {}

	std::array<T, N> getValues() const
	{
		return cValues;
	}

	Candidate crossWith(const Candidate& mate) const
	{
		std::array<T, N> newValues;
		for (auto i = 0; i < N; i++)
		{
			const auto alpha = getRandomBetween0And1();
			newValues[i] = alpha * cValues[i] + static_cast<T>(1.0 - alpha) * mate.cValues[i];
		}

		return Candidate(newValues);
	}

	void mutate()
	{
		const auto shouldMutate = getRandomBetween0And1() < cMutationChance;
		
		if (!shouldMutate)
			return;

		for (auto& value : cValues)
		{
			const auto mutationValue = getRandomBetween0And1() * (cMaxValue - cMinValue) - cMinValue;
			const auto newValue = value + mutationValue;
			value = std::max(std::min(newValue, cMaxValue), cMinValue);
		}
	}

	T operator [](const int index) const
	{
		return cValues[index];
	}

	T getFitting(const std::function<T(const std::array<T, N>)>& function) const
	{
		return static_cast<T>(1 / (function(cValues) + 1));
	}

public:
	static void setMaxValue(const T maxValue)
	{
		cMaxValue = maxValue;
	}
	static void setMinValue(const T minValue)
	{
		cMinValue = minValue;
	}
	static void setChanceToMutate(const T mutationChance)
	{
		cMutationChance = cMutationChance;
	}
private:
	inline static T cMaxValue = 0.0f;
	inline static T cMinValue = 0.0f;
	inline static T cMutationChance = 0.01f;

	inline static std::random_device cRandomDevice;
	inline static std::seed_seq cSeed;
	inline static std::mt19937 cGenerator = std::mt19937(cSeed);
	inline static std::uniform_real_distribution<T> cDistribution =
		std::uniform_real_distribution<T>(0.0f, 1.0f);

	inline static T getRandomBetween0And1()
	{
		return cDistribution(cGenerator);
	}

private:
	std::array<T, N> cValues;
};