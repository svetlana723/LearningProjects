from enum import Enum
from typing import List
import random
import time

# constants
QUESTION_COUNT = 3
PHI = 1.618
BAD_MOOD_PROBABILITY = 1/8
GOOD_MOOD_PROBABILITY = 1/4
CONTINUE_PROBABILITY = 1/3
LUNCH_TIME_MIN = 12
LUNCH_TIME_MAX = 18
LUNCH_TIME_AFTER = 30

class PersonGender(Enum):
	MALE = "М"
	FEMALE = "Ж"
	
class StudentStatus(Enum):
	WAITING = "Очередь"
	PASSED = "Сдал"
	FAILED = "Провалил"

	def __lt__(self, other):
		order = [self.WAITING, self.PASSED, self.FAILED]
		return order.index(self) < order.index(other)
	
class Question:
	def __init__(self, question):
		self.question = question
		self.words = question.split()
		self.correct_answers = 0

class Person:
	def __init__(self, name, gender):
		self.name = name
		self.gender = gender
	
	def get_answer(self, words):
		probabilities = []
		prev_part = 0
		for i in range(len(words) - 1):
			p = (1 - prev_part)/PHI
			prev_part += p
			probabilities.append(p)
		probabilities.append(1 - prev_part)
		if self.gender == PersonGender.FEMALE:
			probabilities.reverse()

		answer = random.choices(words, probabilities)[0]
		return answer
	
class Examiner(Person):
	def __init__(self, name, gender):
		super().__init__(name, gender)
		self.student_index = 0
		self.student_count = 0
		self.failed_count = 0
		self.working_time = 0
		self.current_student = None
		self.had_lunch = False
		
	def get_result(self, correct_count, wrong_count):
		mood_random = random.random()
		if mood_random < BAD_MOOD_PROBABILITY:
			exam_passed = False
		elif mood_random < BAD_MOOD_PROBABILITY + GOOD_MOOD_PROBABILITY:
			exam_passed = True
		else:
			exam_passed = correct_count > wrong_count
		return exam_passed

	def get_correct_answers(self, question_words):
		correct_answers = []
		add_answers = True

		rest_words = question_words[:]
		while len(rest_words) > 0 and add_answers:
			correct_answer = self.get_answer(rest_words)
			correct_answers.append(correct_answer)
			rest_words.remove(correct_answer)
			add_answers = (random.random() < CONTINUE_PROBABILITY)
			
		return correct_answers

	def update_time(self):
		name_len = len(self.name)
		self.working_time += random.uniform(name_len - 1, name_len + 1)

	def check_lunch(self, elapsed_time):
		return (not self.had_lunch and elapsed_time > LUNCH_TIME_AFTER)

	def go_lunch(self):
		lunch_time = random.uniform(LUNCH_TIME_MIN, LUNCH_TIME_MAX)
		time.sleep(lunch_time)
		self.had_lunch = True

	def select_student(self, student):
		self.current_student = student
		self.student_count += 1

	def take_exam(self, student, questions: List[Question]):
		if student is not None:
			exam_time = random.uniform(len(self.name)-1, len(self.name)+1) 
			time.sleep(exam_time)

			selected = random.sample(questions, QUESTION_COUNT)
			correct_count = 0
			wrong_count = 0
			for question in selected:
				correct_answers = self.get_correct_answers(question.words)
				student_answer = student.get_answer(question.words)

				if student_answer in correct_answers:
					correct_count += 1
					question.correct_answers += 1
				else:
					wrong_count += 1

			exam_passed = self.get_result(correct_count, wrong_count)
			student.exam_time = exam_time
			if exam_passed:
				student.status = StudentStatus.PASSED
			else:
				student.status = StudentStatus.FAILED
				self.failed_count += 1
			
			self.current_student = None
			self.update_time()

class Student(Person):
	def __init__(self, name, gender):
		super().__init__(name, gender)
		self.status = StudentStatus.WAITING
		self.exam_time = 0