#pragma once
/// <summary>
/// model of time using integers
/// </summary>
class IntTime
{
public:
	enum TimeComponent { SEC, MIN, HOUR, DAY };
	int secs = 0, mins = 0, hours = 0, days = 0;

	IntTime() {
		secs = 0; mins = 0; hours = 0; days = 0;
	};
	IntTime(TimeComponent component, int value);
	// format [days]d[hours]h[mins]m[secs]s
	void update();
	// adds a second and update
	void tick();
	// return any time to 0d0h0m0s
	void reset();
	// convert any time into seconds only
	int convertToSecs();
	/// <summary>
	/// display IntTime in [days]d[hours]h[mins]m[secs]s.
	/// If any components except for #secs is 0, it will not be displayed.
	/// </summary>
	void display();
	// compares current IntTime to #otherTime
	bool isLessThan(IntTime otherTime);
	// allow to add two IntTime together using "+"
	IntTime& operator+(const IntTime& oldTime);
};

