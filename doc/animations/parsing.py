from manim import *
RED = '#CC0000'
GREY= '#232323'

def update_step(scene, step, text):
  next_step = MathTex(r"\text{" + text + "}")
  next_step.move_to(2 * UP)
  scene.play(Transform(step, next_step))

class ReactionParse(MovingCameraScene):
    def construct(self):
        self.camera.background_color = GREY

        self.play(self.camera.frame.animate.set(width=11))

        A = Text("A")
        A.move_to(3 * LEFT)
        plus_lhs= Text("+")
        plus_lhs.move_to(2* LEFT)
        B = Text("B")
        B.move_to(LEFT)
        arrow = Text("->")

        C = Text("C")
        C.move_to(RIGHT)
        plus_rhs= Text("+")
        plus_rhs.move_to(2 * RIGHT)
        D = Text("D")
        D.move_to(3 * RIGHT)

        self.play(Create(A), Create(plus_lhs), Create(B), Create(arrow), Create(C), Create(plus_rhs), Create(D))

        step = Text('''Split by '_->_' ''')
        step.move_to(2 * UP)

        self.play(Create(step))

        self.play(arrow.animate.set_fill(opacity=0))
        self.wait(0.5)
        A.set_color(RED)
        B.set_color(RED)
        C.set_color(BLUE)
        D.set_color(BLUE)


        next_step = Text(('''Split by '_+_' '''))
        next_step.move_to(2 * UP)
        self.play(Transform(step, next_step))
        self.play(plus_lhs.animate.set_fill(opacity=0), plus_rhs.animate.set_fill(opacity=0))
        self.wait(0.5)
        B.set_color(PURPLE)
        D.set_color(YELLOW)
        next_step = Text(('''Parse each species'''))
        next_step.move_to(2 * UP)
        self.play(Transform(step, next_step))
        self.wait(0.5)
        next_step = Text(('''Validate the Reaction '''))
        next_step.move_to(2 * UP)
        self.play(Transform(step, next_step))
        self.wait(0.5)

        plus_lhs.set_color(GREEN)
        arrow.set_color(GREEN)
        plus_rhs.set_color(GREEN)

        A.set_color(GREEN)
        self.play(plus_lhs.animate.set_fill(opacity=1))
        B.set_color(GREEN)
        self.play(arrow.animate.set_fill(opacity=1))
        C.set_color(GREEN)
        self.play(plus_rhs.animate.set_fill(opacity=1))
        D.set_color(GREEN)
        self.wait(0.5)

        checkmark = Tex(r"\checkmark").scale(5)

        # Set the color of the checkmark to green
        checkmark.set_color(GREEN)

        self.play(A.animate.set_fill(opacity=0),
                  B.animate.set_fill(opacity=0),
                  plus_lhs.animate.set_fill(opacity=0),
                  plus_rhs.animate.set_fill(opacity=0),
                  arrow.animate.set_fill(opacity=0),
                  C.animate.set_fill(opacity=0),
                  D.animate.set_fill(opacity=0)
                  )

        # Display the checkmark
        self.play(Create(checkmark))
        self.wait(2)


class SpeciesParse(MovingCameraScene):
    def construct(self):
        self.camera.background_color = GREY

        self.play(self.camera.frame.animate.set(width=11))

        coef = MathTex("N")
        coef.move_to(1 * LEFT)
        s = MathTex(r"A")
        s.move_to(0.7 * LEFT)
        subs = MathTex(r"n")
        subs.move_to(0.2 * LEFT + 0.05 * DOWN)

        charge = MathTex(r"\pm")
        charge.move_to(0.25 * RIGHT)
        charge_num = MathTex("m")
        charge_num.move_to(0.7 * RIGHT + 0.05 * DOWN)

        mod_brace = MathTex(r"(")
        mod_brace.move_to(1.15 * RIGHT + 0.05 * DOWN)
        mod = MathTex(r"\text{modifier})")
        mod.move_to(2.2 * RIGHT + 0.05 * DOWN)

        self.play(Create(coef), Create(s),Create(subs), Create(charge), Create(mod), Create(charge_num), Create(mod_brace))
        self.wait(0.5)

        step = Text("Find the first capital letter, 'e', or 'hnu'")
        step.move_to(2 * UP)
        self.play(Create(step))
        s.set_color(YELLOW)
        self.wait(1)


        s.set_color(WHITE)
        update_step(self, step, "Anything before this is the coefficient")
        coef.set_color(YELLOW)
        self.play(coef.animate.move_to(4 * LEFT))
        self.wait(0.5)

        update_step(self, step, "The coefficient must be an integer")
        self.wait(0.5)
        coef.set_color(WHITE)

        update_step(self, step, "Find the next non-letter")
        subs.set_color(YELLOW)
        self.wait(1)
        subs.set_color(WHITE)

        update_step(self, step, "Anything before this is the Species Base")
        s.set_color(YELLOW)
        self.play(s.animate.move_to(2.5 * LEFT))
        self.wait(1)
        s.set_color(WHITE)

        update_step(self, step, "Find the next special character")
        charge.set_color(YELLOW)
        self.wait(1)
        charge.set_color(WHITE)


        update_step(self, step, "Anything before is the subscript")
        subs.set_color(YELLOW)
        self.play(subs.animate.move_to(1 * LEFT))
        self.wait(0.5)
        subs.set_color(WHITE)


        update_step(self, step, "Find the next non $\pm$ special character")
        mod_brace.set_color(YELLOW)
        self.wait(1)
        mod_brace.set_color(WHITE)
        self.wait(0.5)
        update_step(self, step, "Any number before is the charge number")
        charge_num.set_color(YELLOW)
        charge.set_color(YELLOW)
        # self.play(charge_num.animate.move_to(0.5 * LEFT))
        self.wait(1)
        charge.set_color(WHITE)
        charge_num.set_color(WHITE)

        update_step(self, step, "Everything else is the modifier")
        mod_brace.set_color(YELLOW)
        mod.set_color(YELLOW)
        self.play(mod.animate.move_to(2.2 * RIGHT + 0.05 * DOWN + 1 * RIGHT),
        mod_brace.animate.move_to(1.15 * RIGHT + 0.05 * DOWN + 1 * RIGHT))

        self.wait(0.5)








