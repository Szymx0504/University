% int: nc = 3;
% var 1..nc: wa; var 1..nc: nt; var 1..nc: sa;   var 1..nc: q; var 1..nc: nsw;  var 1..nc: v;   var 1..nc: t;
% constraint wa != nt;
% constraint wa != sa;
% constraint nt != sa;
% constraint nt != q;
% constraint sa != q;
% constraint sa != nsw;
% constraint sa != v;
% constraint q != nsw;
% constraint nsw != v;
% constraint wa = 1;
% constraint nt = 1;
% solve satisfy;
% output ["wa=\(wa)\t nt=\(nt)\t sa=\(sa)\n",
%         "q=\(q)\t nsw=\(nsw)\t v=\(v)\n",
%          "t=", show(t),  "\n"];

% enum Colour = {red, green, blue};
% var Colour: wa; var Colour: nt; var Colour: sa;   var Colour: q; var Colour: nsw;  var Colour: v;   var Colour: t;
% constraint wa != nt /\ wa != sa /\ nt != sa /\ nt != q /\ sa != q /\ sa != nsw /\ sa != v /\ q != nsw /\ nsw != v;
% constraint wa == red \/ wa == green;
% solve satisfy;

% enum Colour = {red, green, blue};
% enum States = {WA, NSW, NT, V, SA, Q, T};
% array[States] of var Colour: coloring;
% constraint coloring[WA] != coloring[NT] /\ coloring[WA] != coloring[SA] /\ coloring[NT] != coloring[SA] /\ coloring[NT] != coloring[Q] /\ coloring[SA] != coloring[Q] /\
%            coloring[SA] != coloring[NSW] /\ coloring[SA] != coloring[V] /\ coloring[Q] != coloring[NSW] /\ coloring[NSW] != coloring[V];
% solve satisfy;

% enum Colour = {red, green, blue};
% enum States = {WA, NSW, NT, V, SA, Q, T};
% array[States] of var Colour: coloring;
% array[_] of tuple(States, States): neighbourhood = [(WA, NT), (WA, SA), (NT, SA), (NT, Q), (SA, Q), (SA, NSW), (SA, V), (Q, NSW), (V, NSW)];
% constraint forall(n in neighbourhood)(coloring[n.1] != coloring[n.2]);
% solve satisfy;

enum Colour = {red, green, blue};
enum States = {WA, NSW, NT, V, SA, Q, T};
array[States] of var Colour: coloring;
array[States] of set of States: neighbourhood = [{NT, SA}, {}, {SA, Q}, {NSW}, {Q, NSW, V}, {NSW}, {}];
constraint forall(s in States)(forall(r in neighbourhood[s])(coloring[s] != coloring[r]));






