
const util = require('util')
function prt(v) {
	console.log(util.inspect(v, {showHidden: false, depth: null}))
}

function tickets(peopleInLine)
{
	var defs = function() {
		return {
			$100 : { count: 0, value: 100 },
			$50  : { count: 0, value: 50 },
			$25  : { count: 0, value: 25 },
		};
	};

	function divmod(x, y) {
		var div = Math.trunc(x/y);
		var rem = x % y;
		return { div: div, rem: rem };
	}

	var bills = defs();
	var giveChange = function( value ) {
		var change = defs();
		for (denom in bills) {
			if (value == 0)
				return change;
			var bill = bills[denom];
			if (bill.value > value)
				continue;
			if (bill.count == 0)
				continue;
			var needed = divmod(value, bill.value);
			if (needed.div <= bill.count) {
				bill.count          -= needed.div;
				change[denom].count += needed.div;
				value = needed.rem;
			} else {
				value -= bill.value * bill.count;
				change[denom].count += bill.count;
				bill.count = 0;
			}
		};

		if (value == 0)
			return change;
		return null;
	};

	prt(bills);
	for (note of peopleInLine) {
		prt(note);
		var change = giveChange(note - 25);
		prt(change);
		if (change == null) return "NO";
		bills['$' + note].count += 1;
		prt(bills);
	}
	return "YES"; 
}

prt(tickets([25, 25, 50, 50]));
prt(tickets([25, 100]));
