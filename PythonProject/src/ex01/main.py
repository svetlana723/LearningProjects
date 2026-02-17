import multiprocessing
import time
import os
from queue import Empty
from typing import List
from datetime import datetime
from objects import Question, Student, PersonGender, Examiner, StudentStatus
from prettytable import PrettyTable

# constants
STUDENTS_FILE_PATH = "students.txt"
EXAMINERS_FILE_PATH = "examiners.txt"
QUESTIONS_FILE_PATH = "questions.txt"
PASSED_PART = 0.85

def exam_process(examiner: Examiner, students_queue: multiprocessing.Queue, results_queue: multiprocessing.Queue, questions: list, exam_start_time):
	while True:
		try:
			elapsed_time = time.time() - exam_start_time
			if examiner.check_lunch(elapsed_time):
				results_queue.put((examiner, student, questions))
				examiner.go_lunch()
				results_queue.put((examiner, student, questions))

			student : Student = students_queue.get(timeout=1)

			examiner.select_student(student)
			results_queue.put((examiner, student, questions))
			examiner.take_exam(student, questions)
			results_queue.put((examiner, student, questions))
			
		except Empty:
			break
		except Exception as e:
			print(f"Ошибка у экзаменатора {examiner.name}: {e}")
			break

def get_wating_students_count(students):
	return sum(1 for student in students if student.status == StudentStatus.WAITING)

def print_state(examiners, students, elapsed_time):
	os.system('cls' if os.name == 'nt' else 'clear')
	
	student_table = PrettyTable(["Студент", "Статус"])
	for student in sorted(students, key=lambda s: s.status):
		student_table.add_row([student.name, student.status.value])
	print(student_table)

	examiner_table = PrettyTable(["Экзаменатор", "Текущий студент", "Всего студентов", "Завалил", "Время работы"])
	for examiner in examiners:
		examiner_table.add_row([examiner.name, "-" if examiner.current_student is None else examiner.current_student.name, examiner.student_count, examiner.failed_count, f"{examiner.working_time:.2f}"])
	print(examiner_table)

	print(f"Осталось в очереди: {get_wating_students_count(students)} из {len(students)}")
	print(f"Время с момента начала экзамена: {elapsed_time:.2f}")

def print_result(examiners, students, questions, elapsed_time):
	os.system('cls' if os.name == 'nt' else 'clear')

	student_table = PrettyTable(["Студент", "Статус"])
	for student in sorted(students, key=lambda s: s.status):
		student_table.add_row([student.name, student.status.value])
	print(student_table)

	examiner_table = PrettyTable(["Экзаменатор", "Всего студентов", "Завалил", "Время работы"])
	for examiner in examiners:
		examiner_table.add_row([examiner.name, examiner.student_count, examiner.failed_count, f"{examiner.working_time:.2f}"])
	print(examiner_table)

	print(f"Время с момента начала экзамена и до момента и его завершения: {elapsed_time:.2f}")

	items = []
	if any(student.status == StudentStatus.PASSED for student in students):
		min_time = min(student.exam_time for student in students if student.status == StudentStatus.PASSED)
		for student in students:
			if student.status == StudentStatus.PASSED and student.exam_time == min_time:
				items.append(student.name)
		print(f"Имена лучших студентов: {', '.join(items)}")
	
	items.clear()
	min_failed = min(examiner.failed_count/examiner.student_count for examiner in examiners)
	for examiner in examiners:
		if examiner.failed_count/examiner.student_count == min_failed:
			items.append(examiner.name)
	print(f"Имена лучших экзаменаторов: {', '.join(items)}")
	
	if any(student.status == StudentStatus.FAILED for student in students):
		items.clear()
		min_time = min(student.exam_time for student in students if student.status == StudentStatus.FAILED)
		for student in students:
			if student.status == StudentStatus.FAILED and student.exam_time == min_time:
				items.append(student.name)
		print(f"Имена студентов, которых после экзамена отчислят: {', '.join(items)}")

	items = []
	max_count = max(question.correct_answers for question in questions)
	for question in questions:
		if question.correct_answers == max_count:
			items.append('"' + question.question + '"')
	print(f"Лучшие вопросы: {', '.join(items)}")

	passed_students_count = sum(1 for student in students if student.status == StudentStatus.PASSED)
	if passed_students_count/len(students) > PASSED_PART:
		print(f"Вывод: экзамен удался (процент сдавших = {passed_students_count/len(students):.2f})")
	else:
		print(f"Вывод: экзамен не удался (процент сдавших = {passed_students_count/len(students):.2f})")

def read_persons_from_file(filepath):
	items = []
	with open(filepath, 'r', encoding='utf-8') as f:
		items = [line.strip().split() for line in f.readlines()]
	return items

def read_questions_from_file(filepath):
	items = []
	with open(filepath, 'r', encoding='utf-8') as f:
		items = [line.strip() for line in f.readlines()]
	return items

def read_from_files():
	examiners = []
	list_from_file = read_persons_from_file(EXAMINERS_FILE_PATH)
	for item in list_from_file:
		try:
			if item[1] in ["Ж", "М"]:
				examiners.append(Examiner(item[0], PersonGender.FEMALE if item[1] == "Ж" else PersonGender.MALE))
		except Exception as e:
			continue

	students = []
	list_from_file = read_persons_from_file(STUDENTS_FILE_PATH)
	for item in list_from_file:
		try:
			students.append(Student(item[0], PersonGender.FEMALE if item[1] == "Ж" else PersonGender.MALE))
		except Exception as e:
			continue

	questions = []
	list_from_file = read_questions_from_file(QUESTIONS_FILE_PATH)
	for item in list_from_file:
		try:
			if len(item) > 0:
				questions.append(Question(item))
		except Exception as e:
			continue

	return examiners, students, questions

def exam_multiprocessing(Examiners: List[Examiner], Students: List[Student], Questions: List[Question]):

	exam_start_time = time.time()
	exam_elapsed_time = None

	students_queue = multiprocessing.Queue()
	result_queue = multiprocessing.Queue()
	for student in Students:
		students_queue.put(student)

	processes = []
	for examiner in Examiners:
		p = multiprocessing.Process(
			target=exam_process,
			args=(examiner, students_queue, result_queue, Questions, exam_start_time)
		)
		processes.append(p)
	
	for p in processes:
		p.start()

	while any(p.is_alive() for p in processes) or not result_queue.empty():
		exam_elapsed_time = time.time() - exam_start_time
		while not result_queue.empty():
			examiner_upd, student_upd, questions_upd = result_queue.get(timeout=1)
			for i in range(len(Students)):
				if Students[i].name == student_upd.name and Students[i].gender == student_upd.gender:
					Students[i] = student_upd
			for i in range(len(Examiners)):
				if Examiners[i].name == examiner_upd.name and Examiners[i].gender == examiner_upd.gender:
					Examiners[i] = examiner_upd
			Questions = questions_upd

		print_state(Examiners, Students, exam_elapsed_time)

	for p in processes:
		p.join()

	print_result(Examiners, Students, Questions, exam_elapsed_time)


def main():
	Examiners, Students, Questions = read_from_files()
	if len(Examiners) > 0 and len(Students) > 0 and len(Questions) > 0:
		exam_multiprocessing(Examiners, Students, Questions)

if __name__ == '__main__':
	main()