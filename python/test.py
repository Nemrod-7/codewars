
whiteplayer = WhitePlayer("Ke8,Rh7 - Kc8")
whitemove = whiteplayer.play("Kb8")
# Your code will determine what the return value is, let's assume it's "Kd8";
#   then black has again only one possible move:
whitemove = whiteplayer.play("Ka8")
# Maybe you decided to return "Kc7"? Good choice!
whitemove = whiteplayer.play("Ka7")
# Maybe you returned "Rh6" so that black can only return back to square a8
whitemove = whiteplayer.play("Ka8")
# If your code is good, it will certainly return "Ra6" here, 
#   at which moment you pass the test... it is a checkmate.
