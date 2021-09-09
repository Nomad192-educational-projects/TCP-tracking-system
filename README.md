# TCP sending system

The system for sending data of the Int type according to the ASN.1 X.609 standard of the BER format over the TCP protocol. Developed as part of a test assignment for the position of a system C/C++programmer. The system is written in C++ using OpenMP to parallelize the server. The system includes a single-threaded client with the possibility of internal verification of encoding/decoding functions and a multi-threaded data processing server that decodes the sent value, remembers it, encodes it again and responds in the same way, then outputs data about the response and success of the operation.

============================================================================

# Система отправки по TCP

Система отправки данных типа Int по стандарту ASN.1 X.609 формата BER по протоколу TCP. Разработана в рамках тестового задания на должность Системного программиста C/C++. Система написана на языке С++ с использованием OpenMP для распараллеливания сервера. Система включает в себя однопоточный клиент, с возможностью внутренней проверки функций кодрования/декодирования и многопоточный сервер обработки данных, который декодирует присланное значение, запомниет, кодирует его заново и отвечает так-же, затем вывод данные о клиете и успешности операции.
