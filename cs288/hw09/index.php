<!DOCTYPE html>
<html lang="en">
  <body>
  	<link rel="stylesheet" href="resources/css/bootstrap.min.css">
  	<link rel="stylesheet" type="text/css" href="resources/css/main.css"/>

	<div class='container'>
		<table id='stocks-table' class='table table-striped table-bordered table-hover'>
    					<?php
       						$servername = "localhost";
       						$username = "cs288";
       						$password = "zipp0";
       						$db = "cs288";
       						$tbl="2017-11-21_10-00-18";
       						echo "<title id='title'>Stocks || $tbl</title>";
       						$conn = new mysqli($servername, $username, $password, $db);
    	   
							if ($conn->connect_error) {
								die("Connection failed: " . $conn->connect_error);
							}

							$sql = "SHOW TABLES";
							$result = $conn->query($sql);
							if ($result->num_rows > 0){
								echo "<div class='dropdown'>";
								echo "<button class='btn btn-primary dropdown-toggle' type='button' data-toggle='dropdown'>Select Table</button>";
								echo "<ul class='dropdown-menu'>";
								while ($row = $result->fetch_row()){
										echo "<li><a href='#''>$row[0]</a></li><br>";
								}
								echo "</ul>";
								echo "</div>";
							}

							$sql = "SELECT * from `$tbl`";
							$result = $conn->query($sql);
							$col_num = $result->field_count;

							if ($result->num_rows > 0) {
								echo "<thead class='thead-inverse'>";
								echo "<tr>";
								// Get column names
								$fields = $result->fetch_fields();
								foreach($fields as $i){
									echo "<th class='text-center'>$i->name</th>";
								}
								echo "</tr>";
								echo "</thead>";
								echo "<tbody>";
								// Get all data
								while ($row = $result->fetch_row()){
									echo "<tr>";
									for ($i=0; $i<$col_num; $i++){
										echo "<td id='row$i'>$row[$i]</td>";
									}
									echo "</tr>";
								}
								echo "</tbody>";
							} else {
								echo "No results were retreived. Please try again.";
							}
							$conn->close();
						?>
		</table>
	</div>
	
	<script src="http://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
	<script src="resources/js/bootstrap.min.js"></script>
	
  </body>

</html> 
