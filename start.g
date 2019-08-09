// This is a comment
// One line comments like this

///*
//    Block comments like this
//*/

// idea: function definition
//   name : (parameters) return_value {code}
start : (int argc, string argv) int {
    // idea: 
    name : "Nick";
    gender : string;
	age : int;
	other_age : 4;

    //age = 15 + 8.4;
	//gender = "male";

    print ("Hello there!\n");
    print ("I am %.", "Nick Francke\n");
    print ("My name is %! My age is %, I'm a % with a % year old daughter.\n", name, age, gender, other_age);

    return 0;
}