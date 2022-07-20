// Number of rounds of firing per 2 seconds.
const roundCount = 10;
const attackType = "Ranged";

function WarnOutpost() { }

WarnOutpost.prototype.Schema =
"<element name='Detectable'>" +
"<ref name='nonNegativeDecimal'/>" +
"</element>";

WarnOutpost.prototype.MAX_PREFERENCE_BONUS = 2;

WarnOutpost.prototype.Init = function () {
	this.currentRound = 0;
	this.archersGarrisoned = 0;
	this.arrowsLeft = 0;
	this.targetUnits = [];
};

WarnOutpost.prototype.OnUpdate = function (msg) {
	warn("Amogus");
};


Engine.RegisterComponentType(IID_WarnOutpost, "WarnOutpost", WarnOutpost);
