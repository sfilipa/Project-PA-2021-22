<h1>📜 CheckFile</h1>

<div style="border: 1px solid black; padding: 10px;">
<p>This project was developed as part of the <strong>Advanced Programming</strong> course in the Computer Science and Engineering degree during the academic year 2021/2022.
<h1>Collaborators:</h1>
<ul>
<li><a href="https://github.com/sfilipa">Sara</a></li>
<li><a href="https://github.com/MFMici">Micael</a></li>
</ul>
<h1>Project Description:</h1>
The checkFile is a file verifier application, written in C, which verifies if the type of a given file matches the one announced by its file extension. It uses the file utility of Linux (man file) to determine the type of a file based on its content.
<br><br>
<h2>Features</h2>
<ul>
<li>Determines the true file type of a file based on its content.</li>
<li>Verifies if the file type matches the announced extension.</li>
<li>Supports the following file types: PDF, GIF, JPG, PNG, MP4, ZIP, and HTML.</li>
<li>If the file type is not supported, the application will indicate that the file type is not supported.</li>
</ul>
<h2>How it Works</h2>
The checkFile application analyzes the file by executing the <strong><i>file</i></strong> utility of Linux using the <strong><i>fork</i></strong> and <strong><i>exec</i></strong> system calls. It captures the standard output and error streams produced by the <strong><i>file</i></strong> utility to determine the file type. It then compares the file type with the announced file extension to verify if they match. If they do not match, the application indicates which extensions are correct.

<h2>Usage</h2>
To use the checkFile application, run the following command:
<ul>
<li>checkFile '<'filename'>'</li>
</ul>
Replace <strong><i><filename></i></strong> with the name of the file you want to verify.
<p>&#8702; <a href="https://github.com/sfilipa/Project-PA-2021-22/blob/main/EI_PA.1S2021-22.proj_checkFile---v2.pdf">Project Statement</a></p>
<p>&#8702; <a href="https://github.com/sfilipa/Project-PA-2021-22/blob/main/PA.proj-2021-2022.2201743-2201757/relatorio_proj_PA_2201743-2201757.pdf">Report</a></p>
<h1>Tools Used</h1>
<ul>
<li>C Programming Language: Used to write the checkFile application.</li>
<li>Linux Environment: Required for running the checkFile application and executing the `file` utility.</li>
<li>fork() and exec() System Calls: Used to execute the `file` utility and capture its standard output and error streams.</li>
</ul>

<h1>Version Control</h1>
<p>This project was developed during the first semester of the 2-year college program, and version control using Git was not yet introduced. Therefore, the project was not version-controlled using Git during development.</p>

<h1>Other Information</h1>
<ul>
  <li><strong>Our project received a grade of 17.52 out of 20.</strong></li>
  <li>This project was developed for the <a href="https://www.ipleiria.pt/curso/licenciatura-em-engenharia-informatica/" rel="nofollow">Computer Engineering degree</a> at <a href="https://www.ipleiria.pt" rel="nofollow">Polytechnic of Leiria</a></li>
</ul>
<p><a href="https://www.ipleiria.pt/estg/" rel="nofollow"><img src="https://camo.githubusercontent.com/f11c2f47a7221ed3eb4c80f84fe7c67414e23377aff6c6af3182c88624fbbbea/68747470733a2f2f7777772e69706c65697269612e70742f6e6f726d617367726166696361732f77702d636f6e74656e742f75706c6f6164732f73697465732f38302f323031372f30392f657374675f682d30312e6a7067" width="300" alt="Escola Superior de Tecnologia e Gestão" title="Escola Superior de Tecnologia e Gestão" data-canonical-src="https://www.ipleiria.pt/normasgraficas/wp-content/uploads/sites/80/2017/09/estg_h-01.jpg" style="max-width: 100%;"></a></p>
</div>
