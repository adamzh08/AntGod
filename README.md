<h1>An interactive evolution simulation game</h1>

<h3>The creation structure</h3>

main constructs World  
-> <ins>World</ins> holds list of Populations, obstacles  
---> <ins>Population</ins> hold World&, list of Ants  
-----> <ins>Ant</ins> holds Population&  

<h3>The usage structure</h3>

main calls World::act() / draw() each frame  
-> calls foreach Population:act() / draw()  
---> calls foreach Ant::act() / draw()  


<h3>Gui with RayGui</h3>
Action buttons in the bottom of the window  
info texts on the right side of the window