#include <iostream>
#include <vector>
#include <set>
#include <map>

namespace preloaded {
    std::vector<std::string> words {"ACT","ADD","ALL","APE","AND","ANN","ANY","ANT","ARE","ART","ASS","BAD","BAR","BAT","BAY","BEE","BIG","BIT","BOB","BOY","BUN","BUT","CAN","CAR","CAT","COT","COW","CUT","DAD","DAY","DEW","DID","DIN","DOG","DON","DOT","DUD","EAR","EAT","EEL","EGG","ERR","EYE","FAG","FAR","FLY","FOR","FUN","FUR","GAY","GET","GOT","GUM","GUN","GUY","GUT","GYM","HAS","HAT","HER","HEY","HIM","HIS","HIT","HOW","HUG","HUN","ICE","INK","ITS","IVE","JAN","JET","JOB","JOT","JOY","KEY","LAP","LAY","LIE","LET","LOG","MAN","MAP","MAY","MEN","MOM","MUD","MUM","NAP","NEW","NOD","NOT","NOW","OAR","ODD","OFF","OLD","ONE","OUR","OUT","PAN","PAL","PAT","PAW","PEN","PET","PIG","PIT","POT","PRO","PUT","QUO","RAG","RAM","RAN","RAP","RAT","RED","RIP","ROD","ROT","RUN","RUT","SAT","SAW","SAY","SEA","SEE","SEX","SHE","SOY","SUN","SUX","TAN","TAT","TEA","THE","TIN","TIP","TIT","TON","TOP","TOO","TWO","URN","USE","VAN","VET","VIP","WAR","WAS","WAY","WED","WHO","WHY","WIN","WON","XXX","YAK","YAM","YAP","YOU","YUM","ZAP","ZIP","ZIT","ZOO","ABLE","ACED","AGOG","AHEM","AHOY","ALLY","AMEN","ANTI","ANTS","ANUS","APES","ARMY","ARSE","ARTY","AVID","AWED","BABY","BARS","BATS","BAYS","BEAR","BEES","BILL","BITE","BITS","BLOW","BLUE","BOLD","BONE","BOOB","BOOM","BOSS","BOYS","BUFF","BUNG","BUNS","BUMS","BURP","BUST","BUSY","BUZZ","CANS","CANT","CARS","CART","CATS","CHAP","CHIC","CHUM","CIAO","CLAP","COCK","CODE","COOL","COWS","COZY","CRAB","CREW","CURE","CULT","DADS","DAFT","DAWN","DAYS","DECK","DEED","DICK","DING","DOGS","DOTS","DOLL","DOLT","DONG","DOPE","DOWN","DRAW","DUCK","DUDE","DUMB","DUTY","EARL","EARN","EARS","EASY","EATS","EDGE","EELS","EGGS","ENVY","EPIC","EYES","FACE","FAGS","FANG","FARM","FART","FANS","FAST","FEAT","FEET","FISH","FIVE","FIZZ","FLAG","FLEW","FLIP","FLOW","FOOD","FORT","FUCK","FUND","GAIN","GEEK","GEMS","GIFT","GIRL","GIST","GIVE","GLEE","GLOW","GOLD","GOOD","GOSH","GRAB","GRIN","GRIT","GROT","GROW","GRUB","GUNS","GUSH","GYMS","HAIL","HAIR","HALO","HANG","HATS","HEAD","HEAL","HEIR","HELL","HELP","HERE","HERO","HERS","HIGH","HIRE","HITS","HOLY","HOPE","HOST","HUNK","HUGE","HUNG","HUNS","HURT","ICON","IDEA","IDLE","IDOL","IOTA","JAZZ","JERK","JESS","JETS","JINX","JOBS","JOHN","JOKE","JUMP","JUNE","JULY","JUNK","JUST","KATA","KEYS","KICK","KIND","KING","KISS","KONG","KNOB","KNOW","LARK","LATE","LEAN","LICE","LICK","LIKE","LION","LIVE","LOGS","LOCK","LONG","LOOK","LORD","LOVE","LUCK","LUSH","MAKE","MANY","MART","MATE","MAXI","MEEK","MIKE","MILD","MINT","MMMM","MOMS","MOOD","MOON","MOOT","MUCH","MUFF","MUMS","MUTT","NAPS","NAZI","NEAT","NECK","NEED","NEWS","NEXT","NICE","NICK","NOON","NOSE","NOTE","OARS","OATS","ONCE","ONLY","OPEN","ORGY","OVAL","OVER","PANS","PALS","PART","PAST","PATS","PAWS","PEAR","PERT","PENS","PETS","PHEW","PIPE","PIPS","PLAN","PLUM","PLUS","POET","POOF","POOP","POSH","POTS","PROS","PSST","PUKE","PUNK","PURE","PUSH","PUSS","QUAD","QUAK","QUID","QUIT","RANT","RAPE","RAPS","RAPT","RATE","RAMS","RATS","REAP","RICK","RING","RIPE","ROOT","ROSE","ROSY","ROTS","RUNT","RUTS","SAFE","SAGE","SANE","SAVE","SAWS","SEEK","SEXY","SHAG","SHIT","SICK","SIGH","SIRE","SLAG","SLIT","SLUT","SNAP","SNOG","SNUG","SOFT","SOON","SOUL","SOUP","SPRY","STIR","STUN","SUCK","SWAG","SWAY","TACT","TANK","TANS","THAT","THIS","TIME","TINS","TINY","TITS","TOES","TONS","TONY","TOPS","TOYS","UBER","URNS","USED","USER","USES","VAIN","VAMP","VARY","VEIN","VENT","VERY","VEST","VIEW","VIVA","VOLT","VOTE","WAFT","WAGE","WAKE","WALK","WALL","WANG","WANK","WANT","WARD","WARM","WARP","WARS","WART","WASH","WAVE","WEAR","WEDS","WEED","WEEN","WELD","WHAT","WHEE","WHEW","WHIP","WHIZ","WHOA","WIFE","WILL","WIND","WING","WINK","WINS","WIRE","WISH","WITH","WORD","WORK","WRAP","XMAN","XMEN","XRAY","XTRA","XXXX","YANK","YAKS","YAMS","YAPS","YARD","YARN","YELP","YERN","YOKE","YOLK","YULE","ZANY","ZAPS","ZIPS","ZITS","ZERO","ZOOM","ZOOS"};
};

std::map<char,int> base = {{'A', 0},{'B', 0},{'C', 0},{'D', 1},{'E', 1},{'F', 1}, {'G', 2},{'H', 2},{'I', 2}, {'J', 3},{'K', 3},{'L', 3}, {'M', 4},{'N', 4},{'O', 4}, {'P', 5},{'Q', 5},{'R', 5},{'S', 5},{'T', 6},{'U', 6},{'V', 6},{'W', 7},{'X', 7},{'Y', 7},{'Z', 7}};

std::vector<std::string> search (std::string str) {
    std::vector<int> num;
    std::vector<std::string> out;

    for (auto &it : str) {
        num.push_back (base[it]);
    }

    for (auto &word : preloaded::words) {
        if (word.size() == str.size()) {
            bool flag = true;

            for (int i = 0; i < word.size(); i++) {
                if (base[word[i]] != num[i]) flag = false;
            }

            if (flag) {
                out.push_back (word);
            }
        }
    }

    return out;
}
std::set<std::string> check1800(const std::string &str) {

    std::string sub = str.substr (6);
    std::vector<std::string> pref = search (sub.substr (0, sub.find ("-")));
    std::vector<std::string> post = search (sub.substr (sub.find ("-") + 1));
    std::set<std::string> out;

    for (auto &fst : pref) {
        for (auto &scd : post) {
            std::string phone = "1-800-" + fst + "-" + scd;
            out.insert (phone);
        }
    }

    return out;
}

int main () {

}
