int: output_len = card(Classes) * card(Groups);
array[1..output_len, 1..5] of var int: target;

% limit indices
constraint forall(i in 1..output_len)(
  target[i, 1] in Slots /\ target[i, 2] in Groups /\ target[i, 3] in Classes /\ target[i, 4] in Teachers /\ target[i, 5] in Rooms
);

constraint forall(i in 1..output_len)(
  forall(j in i+1..output_len)(
    target[i, 1] != target[j, 1] \/ (target[i, 2] != target[j, 2] /\ target[i, 4] != target[j, 4] /\ target[i, 5] != target[j, 5])
  )
);

constraint forall(i in 1..output_len)(
  target[i, 3] in CanTeach[Teachers[target[i, 4]]]
);
constraint forall(i in 1..output_len)(
  target[i, 5] in Suitable[Classes[target[i, 3]]]
);

constraint forall(i in 1..output_len)(
  forall(j in i+1..output_len)(
    target[i, 2] != target[j, 2] \/ target[i, 3] != target[j, 3] % either diff group or class => exactly each combination exists (once)
  )
);
constraint card(Slots) * card(Rooms) >= card(Classes) * card(Groups);

solve satisfy;
output [ show(target[i,1]) ++ " " ++ 
         show(Groups[target[i,2]]) ++ " " ++ 
         show(Classes[target[i,3]]) ++ " " ++ 
         show(Teachers[target[i,4]]) ++ " " ++ 
         show(Rooms[target[i,5]]) ++ "\n" | i in 1..output_len ];