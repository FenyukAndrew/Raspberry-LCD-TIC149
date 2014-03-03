ѕроект дл€ работы с экраном на C++

***** —“ј–ќ≈

 омпил€ци€ одного файла
gcc main.cc -o main

— использованием std заработало только при
g++ main.cc -o main

ѕериодически возникает ошибка
classA.h:1:1: error: unknown type name СclassТ
classA.h:2:1: error: expected С=Т, С,Т, С;Т, СasmТ or С__attribute__Т before С{Т token
–≈Ў≈Ќ»≈: повторна€ перекомпил€ци€
„то-то часто возникают подобные ошибки - нужен ReBuild All?

«апуск sudo ./main


const int* - слева от * - данные
int* const - справа от * - константный указатель
http://books.google.ru/books?id=uPvvxRYj-NIC&pg=PA380&lpg=PA380&dq=C%2B%2B+const+%D1%81%D0%BB%D0%B5%D0%B2%D0%B0+%D0%BE%D1%82+*&source=bl&ots=K5mme-V8bM&sig=tPk_0Vzqx5yivPZRbK9VykQnijY&hl=ru&sa=X&ei=V2n8UsHLF_H44QTw0ICoAQ&ved=0CC8Q6AEwAQ#v=onepage&q=C%2B%2B%20const%20%D1%81%D0%BB%D0%B5%D0%B2%D0%B0%20%D0%BE%D1%82%20*&f=false


//—трауструп - прочитано 5-5.2
„итать 5.3 - Ќо перед этим нужно прочитать главу 3 - т.к. код построен на программе калькул€тор стр. 70


ќсновной проект:
компил€ци€ sudo make
запуск sudo ./prog


