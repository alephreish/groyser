<?php
if (empty($_GET['query'])) {
	$args = getopt('', array('query:'));
	$query = empty($args['query']) ? false : trim($args['query']);
}
else {
	$query = trim($_GET['query']);
}
if (empty($query)) goto bottom;

include 'groyser_class.inc';
$groys_obj = new groyser();
$translates = $groys_obj->translate($query);
if ($translates) {
	include 'groyser_top.inc';
	foreach ($translates as $titl => $translate) {
		print "<div class='dsl_definition'><p></p>\n";
		print "<div class='dsl_headwords'><big>$titl</big></div>\n";
		$definitions = array();
		if ($translate[0] == '[') {
			$parts = preg_split('/[\[\]]/', $translate, 3);
			print "<p>[".$parts[1]."]</p>";
			$translate = $parts[2];
		}
		$definitions = preg_split('/\d+\. /', $translate);
		$mult_def = (count($definitions) > 1);
		print "<div class='dsl_m1'>".$definitions[0]."</div>";
		unset($definitions[0]);
		if ($mult_def) print "<ol>";
		foreach ($definitions as $definition)
			print ($mult_def ? "<li>" : "")."<div class='dsl_m1'>$definition</div>".($mult_def ? "</li>" : "")."\n";
		if ($mult_def) print "</ol>\n";
		print "</div>";
	}
	include 'groyser_bottom.inc';
}
elseif (!empty($groys_obj->error)) print $groys_obj->error;
bottom:
?>
